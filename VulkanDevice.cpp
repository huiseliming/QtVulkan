#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanTools.h"
#include <memory>
#include <vector>
#include <set>
#include <cassert>
#include <vulkan/vulkan_core.h>

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VkPhysicalDevice physicalDevice)
{
    vkGetPhysicalDeviceProperties(physicalDevice, &_PhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &_PhysicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &_PhysicalDeviceMemoryProperties);
    uint32_t _QueueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_QueueFamilyPropertiesCount, nullptr);
    _QueueFamilyProperties.resize(_QueueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_QueueFamilyPropertiesCount, _QueueFamilyProperties.data());

    uint32_t DeviceExtensionPropertiesCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &DeviceExtensionPropertiesCount, nullptr);
    _SupportedExtensionProperties.resize(DeviceExtensionPropertiesCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &DeviceExtensionPropertiesCount, _SupportedExtensionProperties.data());
}

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VulkanPhysicalDeviceInfo &&other)
{
    this->_PhysicalDevice = other._PhysicalDevice;
    this->_PhysicalDeviceProperties = other._PhysicalDeviceProperties;
    this->_PhysicalDeviceFeatures = other._PhysicalDeviceFeatures;
    this->_PhysicalDeviceMemoryProperties = other._PhysicalDeviceMemoryProperties;
    this->_QueueFamilyProperties = std::move(other._QueueFamilyProperties);
    this->_SupportedExtensionProperties = std::move(other._SupportedExtensionProperties);
}

VulkanPhysicalDeviceInfo &VulkanPhysicalDeviceInfo::operator=(VulkanPhysicalDeviceInfo && other)
{
    if(std::addressof(other) != this){
        this->_PhysicalDevice = other._PhysicalDevice;
        this->_PhysicalDeviceProperties = other._PhysicalDeviceProperties;
        this->_PhysicalDeviceFeatures = other._PhysicalDeviceFeatures;
        this->_PhysicalDeviceMemoryProperties = other._PhysicalDeviceMemoryProperties;
        this->_QueueFamilyProperties = std::move(other._QueueFamilyProperties);
        this->_SupportedExtensionProperties = std::move(other._SupportedExtensionProperties);
    }
    return *this;
}

uint64_t VulkanPhysicalDeviceInfo::GetDeviceLocalMemorySize()
{
    uint64_t deviceLocalMemorySize = 0;
    std::set<int32_t> deviceLocalMemoryHeaps;
    for(uint32_t i = 0; i < _PhysicalDeviceMemoryProperties.memoryTypeCount; i++){
        if(_PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT){
            if(_PhysicalDeviceMemoryProperties.memoryHeaps[_PhysicalDeviceMemoryProperties.memoryTypes[i].heapIndex].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT){
                deviceLocalMemoryHeaps.insert(_PhysicalDeviceMemoryProperties.memoryTypes[i].heapIndex);
            }
        }
    }
    for (auto& deviceLocalMemoryHeap : deviceLocalMemoryHeaps){
        deviceLocalMemorySize += _PhysicalDeviceMemoryProperties.memoryHeaps[deviceLocalMemoryHeap].size;
    }
    return deviceLocalMemorySize;
}

std::optional<uint32_t> VulkanPhysicalDeviceInfo::GetGraphicsQueueIndex()
{
    VkQueueFlags graphicsQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT;
    for (uint32_t i = 0; i < _QueueFamilyProperties.size(); i++) {
        if ((_QueueFamilyProperties[i].queueFlags & graphicsQueueFlags) == graphicsQueueFlags) {
            return i;
        }
    }
    return std::optional<uint32_t>();
}

std::optional<uint32_t> VulkanPhysicalDeviceInfo::GetComputeQueueIndex()
{
    VkQueueFlags computeQueueFlags = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT;
    for (uint32_t i = 0; i < _QueueFamilyProperties.size(); i++) {
        if ((_QueueFamilyProperties[i].queueFlags & computeQueueFlags) == computeQueueFlags &&
                !(_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            return i;
        }
    }
    return std::optional<uint32_t>();
}

std::optional<uint32_t> VulkanPhysicalDeviceInfo::GetTransferQueueIndex()
{
    VkQueueFlags TransferQueueFlags =  VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT;
    for (uint32_t i = 0; i < _QueueFamilyProperties.size(); i++) {
        if ((_QueueFamilyProperties[i].queueFlags & TransferQueueFlags) == TransferQueueFlags &&
                !(_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                !(_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            return i;
        }
    }
    return std::optional<uint32_t>();
}

void VulkanDevice::CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*> enabledLayerNames, std::vector<const char*> enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(_Device == VK_NULL_HANDLE);
    assert(surface != VK_NULL_HANDLE);
    static char errormsg[1024];
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const float defaultQueuePriority = 0.0f;
    // make queue create info
    std::optional<uint32_t> graphicsQueueIndex = physicalDeviceInfo.GetGraphicsQueueIndex();
    std::optional<uint32_t> computeQueueIndex = physicalDeviceInfo.GetComputeQueueIndex();
    std::optional<uint32_t> transferQueueIndex = physicalDeviceInfo.GetTransferQueueIndex();
    // Graphics queue
    if (!graphicsQueueIndex.has_value()){
        sprintf(errormsg, "No graphics queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Compute queue
    if (!computeQueueIndex.has_value()){
        sprintf(errormsg, "No compute queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Dedicated transfer queue
    if (!transferQueueIndex.has_value()){
        sprintf(errormsg, "No transfer queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    queueFamilyIndices.graphics = graphicsQueueIndex.value();
    queueFamilyIndices.compute = computeQueueIndex.value();
    queueFamilyIndices.transfer = transferQueueIndex.value();

    queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndices.graphics,
        .queueCount = 1,
        .pQueuePriorities = &defaultQueuePriority,
    });
    if (queueFamilyIndices.compute != queueFamilyIndices.graphics) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamilyIndices.compute,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }
    if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) &&
        (queueFamilyIndices.transfer != queueFamilyIndices.compute)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamilyIndices.transfer,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }

    // find a present queue
    {
        uint32_t queueFamilyCount = physicalDeviceInfo._QueueFamilyProperties.size();
        std::vector<VkBool32> supportsPresent(queueFamilyCount);
        for (uint32_t i = 0; i < queueFamilyCount; i++){
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDeviceInfo._PhysicalDevice, i, surface, &supportsPresent[i]);
        }
        uint32_t graphicsQueueIndex = UINT32_MAX;
        uint32_t presentQueueIndex = UINT32_MAX;
        // find a queue for present and graphics
        if(supportsPresent[queueFamilyIndices.graphics] == VK_TRUE){
            presentQueueIndex = queueFamilyIndices.graphics;
        }
        // find a queue for present and graphics
        if (presentQueueIndex == UINT32_MAX) {
            for (uint32_t i = 0; i < queueFamilyCount; i++) {
                if ((physicalDeviceInfo._QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                    if (supportsPresent[i] == VK_TRUE) {
                        graphicsQueueIndex = i;
                        presentQueueIndex = i;
                        break;
                    }
                }
            }
        }
        // find a queue just present
        if (presentQueueIndex == UINT32_MAX) {
            for (uint32_t i = 0; i < queueFamilyCount; ++i) {
                if (supportsPresent[i] == VK_TRUE) {
                    presentQueueIndex = i;
                    break;
                }
            }
        }

        if (presentQueueIndex == UINT32_MAX) {
            sprintf(errormsg, "No transfer queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
            VK_THROW_EXCEPT(errormsg);
        }
        if (graphicsQueueIndex != UINT32_MAX) {
            queueFamilyIndices.graphics = graphicsQueueIndex;
        }
        queueFamilyIndices.present = presentQueueIndex;
    }

    if ((queueFamilyIndices.present != queueFamilyIndices.graphics) &&
        (queueFamilyIndices.present != queueFamilyIndices.compute ) &&
        (queueFamilyIndices.present != queueFamilyIndices.transfer)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamilyIndices.present,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }

    auto it = std::find_if(enabledExtensionNames.begin(), enabledExtensionNames.end(), [](const char* enabledExtensionName){
        return strcmp(enabledExtensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
    });

    if(it != enabledExtensionNames.end()){
        enabledExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }


    VkDeviceCreateInfo deviceCI{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledLayerCount = static_cast<uint32_t>(enabledLayerNames.size()),
        .ppEnabledLayerNames = enabledLayerNames.data(),
        .enabledExtensionCount = static_cast<uint32_t>(enabledExtensionNames.size()),
        .ppEnabledExtensionNames = enabledExtensionNames.data(),
        .pEnabledFeatures = &enabledFeatures,
    };

    VK_ASSERT_SUCCESSED(vkCreateDevice(_PhysicalDevice, &deviceCI, nullptr, &_Device));

    std::vector<uint32_t> allIndices{
        queueFamilyIndices.graphics,
        queueFamilyIndices.compute,
        queueFamilyIndices.transfer,
        queueFamilyIndices.present
    };
    std::vector<VkQueue *> queues{&graphicsQueue_, &computeQueue_, &transferQueue_, &presentQueue_};
    std::vector<uint32_t> queueIndices(allIndices.size(), UINT32_MAX);
    for (size_t i = 0; i < allIndices.size(); i++) {
        if (allIndices[i] != UINT32_MAX) {
            bool exist = false;
            for (size_t j = 0; j < queueIndices.size(); j++) {
                if (queueIndices[j] == allIndices[i]) {
                    exist = true;
                    *queues[i] = *queues[j];
                }
            }
            if (!exist) {
                queueIndices[i] = allIndices[i];
                vkGetDeviceQueue(logicalDevice, allIndices[i], 0, queues[i]);
            } else {
            }
        }
    }
}

void VulkanDevice::DestroyDevice() 
{
    if(_Device != VK_NULL_HANDLE){
        vkDestroyDevice(_Device,nullptr);
    }
}

std::vector<VkPhysicalDevice> VulkanDevice::GetPhysicalDevices(VulkanInstance& instance)
{
    std::vector<VkPhysicalDevice> physicalDevices;
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    physicalDevices.resize(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
    return physicalDevices;
}
