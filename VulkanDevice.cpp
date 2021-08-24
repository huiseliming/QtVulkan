#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanTools.h"
#include <memory>
#include <vector>
#include <set>
#include <cassert>
#include <vulkan/vulkan_core.h>

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VkPhysicalDevice physicalDevice)
    :_PhysicalDevice(physicalDevice)
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

    uint32_t DeviceLayerPropertiesCount = 0;
    vkEnumerateDeviceLayerProperties(physicalDevice, &DeviceLayerPropertiesCount, nullptr);
    _SupportedLayerProperties.resize(DeviceLayerPropertiesCount);
    vkEnumerateDeviceLayerProperties(physicalDevice, &DeviceLayerPropertiesCount, _SupportedLayerProperties.data());
}

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VulkanPhysicalDeviceInfo &&other)
{
    assert(std::addressof(other) != this);
    this->_PhysicalDevice = other._PhysicalDevice;
    this->_PhysicalDeviceProperties = other._PhysicalDeviceProperties;
    this->_PhysicalDeviceFeatures = other._PhysicalDeviceFeatures;
    this->_PhysicalDeviceMemoryProperties = other._PhysicalDeviceMemoryProperties;
    this->_QueueFamilyProperties = std::move(other._QueueFamilyProperties);
    this->_SupportedExtensionProperties = std::move(other._SupportedExtensionProperties);
    this->_SupportedLayerProperties = std::move(other._SupportedLayerProperties);
}

VulkanPhysicalDeviceInfo &VulkanPhysicalDeviceInfo::operator=(VulkanPhysicalDeviceInfo && other)
{
    assert(std::addressof(other) != this);
    this->_PhysicalDevice = other._PhysicalDevice;
    this->_PhysicalDeviceProperties = other._PhysicalDeviceProperties;
    this->_PhysicalDeviceFeatures = other._PhysicalDeviceFeatures;
    this->_PhysicalDeviceMemoryProperties = other._PhysicalDeviceMemoryProperties;
    this->_QueueFamilyProperties = std::move(other._QueueFamilyProperties);
    this->_SupportedExtensionProperties = std::move(other._SupportedExtensionProperties);
    this->_SupportedLayerProperties = std::move(other._SupportedLayerProperties);
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

VulkanDevice::~VulkanDevice()
{
    DestroyDevice();
}

void VulkanDevice::CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*> enabledLayerNames, std::vector<const char*> enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(_Device == VK_NULL_HANDLE);
    assert(surface != VK_NULL_HANDLE);
    static char errormsg[1024];
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const float defaultQueuePriority = 0.0f;
    // Make queue create info
    _QueueFamilyIndices.graphics = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo._QueueFamilyProperties, VK_QUEUE_GRAPHICS_BIT);
    _QueueFamilyIndices.compute = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo._QueueFamilyProperties, VK_QUEUE_COMPUTE_BIT);
    _QueueFamilyIndices.transfer = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo._QueueFamilyProperties, VK_QUEUE_TRANSFER_BIT);

    // Graphics queue
    if (_QueueFamilyIndices.graphics == UINT32_MAX){
        sprintf(errormsg, "No graphics queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Compute queue
    if (_QueueFamilyIndices.compute == UINT32_MAX){
        sprintf(errormsg, "No compute queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Dedicated transfer queue
    if (_QueueFamilyIndices.transfer == UINT32_MAX){
        sprintf(errormsg, "No transfer queue available for this device (%s) !", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = _QueueFamilyIndices.graphics,
        .queueCount = 1,
        .pQueuePriorities = &defaultQueuePriority,
    });
    if (_QueueFamilyIndices.compute != _QueueFamilyIndices.graphics) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = _QueueFamilyIndices.compute,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }
    if ((_QueueFamilyIndices.transfer != _QueueFamilyIndices.graphics) &&
        (_QueueFamilyIndices.transfer != _QueueFamilyIndices.compute)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = _QueueFamilyIndices.transfer,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }

    // find a present queue
    {
        uint32_t queueFamilyCount = static_cast<uint32_t>(physicalDeviceInfo._QueueFamilyProperties.size());
        std::vector<VkBool32> supportsPresent(queueFamilyCount);
        for (uint32_t i = 0; i < queueFamilyCount; i++){
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDeviceInfo._PhysicalDevice, i, surface, &supportsPresent[i]);
        }
        uint32_t graphicsQueueIndex = UINT32_MAX;
        uint32_t presentQueueIndex = UINT32_MAX;
        // find a queue for present and graphics
        if(supportsPresent[_QueueFamilyIndices.graphics] == VK_TRUE){
            presentQueueIndex = _QueueFamilyIndices.graphics;
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
        // find a queue just present
        if (presentQueueIndex == UINT32_MAX) {
            sprintf(errormsg, "No persent queue available for device (%s) and surface!", physicalDeviceInfo._PhysicalDeviceProperties.deviceName);
            VK_THROW_EXCEPT(errormsg);
        }
        if (graphicsQueueIndex != UINT32_MAX) {
            _QueueFamilyIndices.graphics = graphicsQueueIndex;
        }
        _QueueFamilyIndices.present = presentQueueIndex;
    }

    if ((_QueueFamilyIndices.present != _QueueFamilyIndices.graphics) &&
        (_QueueFamilyIndices.present != _QueueFamilyIndices.compute ) &&
        (_QueueFamilyIndices.present != _QueueFamilyIndices.transfer)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = _QueueFamilyIndices.present,
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

    VK_ASSERT_SUCCESSED(vkCreateDevice(physicalDeviceInfo._PhysicalDevice, &deviceCI, nullptr, &_Device));

    // get device queue
    std::vector<uint32_t> queueFamilyList{
        _QueueFamilyIndices.graphics,
        _QueueFamilyIndices.compute,
        _QueueFamilyIndices.transfer,
        _QueueFamilyIndices.present
    };
    std::vector<VkQueue *> queues{_GraphicsQueue.AddressOf(), _ComputeQueue.AddressOf(), _TransferQueue.AddressOf(), _PresentQueue.AddressOf()};
    std::vector<uint32_t> queueIndices(queueFamilyList.size(), UINT32_MAX);
    for (size_t i = 0; i < queueFamilyList.size(); i++) {
        if (queueFamilyList[i] != UINT32_MAX) {
            bool exist = false;
            for (size_t j = 0; j < queueIndices.size(); j++) {
                if (queueIndices[j] == queueFamilyList[i]) {
                    exist = true;
                    *queues[i] = *queues[j];
                }
            }
            if (!exist) {
                queueIndices[i] = queueFamilyList[i];
                vkGetDeviceQueue(_Device, queueFamilyList[i], 0, queues[i]);
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
