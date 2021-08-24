#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanTools.h"
#include <memory>
#include <vector>
#include <set>
#include <cassert>
#include <vulkan/vulkan_core.h>

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VkPhysicalDevice physicalDevice)
    :PhysicalDevice(physicalDevice)
{
    vkGetPhysicalDeviceProperties(physicalDevice, &PhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &PhysicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &PhysicalDeviceMemoryProperties);
    uint32_t _QueueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_QueueFamilyPropertiesCount, nullptr);
    QueueFamilyProperties.resize(_QueueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_QueueFamilyPropertiesCount, QueueFamilyProperties.data());

    uint32_t DeviceExtensionPropertiesCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &DeviceExtensionPropertiesCount, nullptr);
    SupportedExtensionProperties.resize(DeviceExtensionPropertiesCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &DeviceExtensionPropertiesCount, SupportedExtensionProperties.data());

    uint32_t DeviceLayerPropertiesCount = 0;
    vkEnumerateDeviceLayerProperties(physicalDevice, &DeviceLayerPropertiesCount, nullptr);
    SupportedLayerProperties.resize(DeviceLayerPropertiesCount);
    vkEnumerateDeviceLayerProperties(physicalDevice, &DeviceLayerPropertiesCount, SupportedLayerProperties.data());
}

VulkanPhysicalDeviceInfo::VulkanPhysicalDeviceInfo(VulkanPhysicalDeviceInfo &&other)
{
    assert(std::addressof(other) != this);
    this->PhysicalDevice = other.PhysicalDevice;
    other.PhysicalDevice = VK_NULL_HANDLE;
    this->PhysicalDeviceProperties = other.PhysicalDeviceProperties;
    this->PhysicalDeviceFeatures = other.PhysicalDeviceFeatures;
    this->PhysicalDeviceMemoryProperties = other.PhysicalDeviceMemoryProperties;
    this->QueueFamilyProperties = std::move(other.QueueFamilyProperties);
    this->SupportedExtensionProperties = std::move(other.SupportedExtensionProperties);
    this->SupportedLayerProperties = std::move(other.SupportedLayerProperties);
}

VulkanPhysicalDeviceInfo &VulkanPhysicalDeviceInfo::operator=(VulkanPhysicalDeviceInfo && other)
{
    assert(std::addressof(other) != this);
    this->PhysicalDevice = other.PhysicalDevice;
    other.PhysicalDevice = VK_NULL_HANDLE;
    this->PhysicalDeviceProperties = other.PhysicalDeviceProperties;
    this->PhysicalDeviceFeatures = other.PhysicalDeviceFeatures;
    this->PhysicalDeviceMemoryProperties = other.PhysicalDeviceMemoryProperties;
    this->QueueFamilyProperties = std::move(other.QueueFamilyProperties);
    this->SupportedExtensionProperties = std::move(other.SupportedExtensionProperties);
    this->SupportedLayerProperties = std::move(other.SupportedLayerProperties);
    return *this;
}

uint64_t VulkanPhysicalDeviceInfo::GetDeviceLocalMemorySize()
{
    uint64_t deviceLocalMemorySize = 0;
    std::set<int32_t> deviceLocalMemoryHeaps;
    for(uint32_t i = 0; i < PhysicalDeviceMemoryProperties.memoryTypeCount; i++){
        if(PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT){
            if(PhysicalDeviceMemoryProperties.memoryHeaps[PhysicalDeviceMemoryProperties.memoryTypes[i].heapIndex].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT){
                deviceLocalMemoryHeaps.insert(PhysicalDeviceMemoryProperties.memoryTypes[i].heapIndex);
            }
        }
    }
    for (auto& deviceLocalMemoryHeap : deviceLocalMemoryHeaps){
        deviceLocalMemorySize += PhysicalDeviceMemoryProperties.memoryHeaps[deviceLocalMemoryHeap].size;
    }
    return deviceLocalMemorySize;
}

VulkanDevice::~VulkanDevice()
{
    Destroy();
}

void VulkanDevice::Create(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*> enabledLayerNames, std::vector<const char*> enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(Device == VK_NULL_HANDLE);
    assert(surface != VK_NULL_HANDLE);
    PhysicalDevice = physicalDeviceInfo.PhysicalDevice;
    static char errormsg[1024];
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const float defaultQueuePriority = 0.0f;
    // Make queue create info
    QueueFamilyIndices.graphics = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo.QueueFamilyProperties, VK_QUEUE_GRAPHICS_BIT);
    QueueFamilyIndices.compute = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo.QueueFamilyProperties, VK_QUEUE_COMPUTE_BIT);
    QueueFamilyIndices.transfer = VulkanTools::GetQueueFamilyIndex(physicalDeviceInfo.QueueFamilyProperties, VK_QUEUE_TRANSFER_BIT);

    // Graphics queue
    if (QueueFamilyIndices.graphics == UINT32_MAX){
        sprintf(errormsg, "No graphics queue available for this device (%s) !", physicalDeviceInfo.PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Compute queue
    if (QueueFamilyIndices.compute == UINT32_MAX){
        sprintf(errormsg, "No compute queue available for this device (%s) !", physicalDeviceInfo.PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    // Dedicated transfer queue
    if (QueueFamilyIndices.transfer == UINT32_MAX){
        sprintf(errormsg, "No transfer queue available for this device (%s) !", physicalDeviceInfo.PhysicalDeviceProperties.deviceName);
        VK_THROW_EXCEPT(errormsg);
    }
    queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = QueueFamilyIndices.graphics,
        .queueCount = 1,
        .pQueuePriorities = &defaultQueuePriority,
    });
    if (QueueFamilyIndices.compute != QueueFamilyIndices.graphics) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = QueueFamilyIndices.compute,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }
    if ((QueueFamilyIndices.transfer != QueueFamilyIndices.graphics) &&
        (QueueFamilyIndices.transfer != QueueFamilyIndices.compute)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = QueueFamilyIndices.transfer,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }

    // find a present queue
    {
        uint32_t queueFamilyCount = static_cast<uint32_t>(physicalDeviceInfo.QueueFamilyProperties.size());
        std::vector<VkBool32> supportsPresent(queueFamilyCount);
        for (uint32_t i = 0; i < queueFamilyCount; i++){
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDeviceInfo.PhysicalDevice, i, surface, &supportsPresent[i]);
        }
        uint32_t graphicsQueueIndex = UINT32_MAX;
        uint32_t presentQueueIndex = UINT32_MAX;
        // find a queue for present and graphics
        if(supportsPresent[QueueFamilyIndices.graphics] == VK_TRUE){
            presentQueueIndex = QueueFamilyIndices.graphics;
        }
        // find a queue for present and graphics
        if (presentQueueIndex == UINT32_MAX) {
            for (uint32_t i = 0; i < queueFamilyCount; i++) {
                if ((physicalDeviceInfo.QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
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
            sprintf(errormsg, "No persent queue available for device (%s) and surface!", physicalDeviceInfo.PhysicalDeviceProperties.deviceName);
            VK_THROW_EXCEPT(errormsg);
        }
        if (graphicsQueueIndex != UINT32_MAX) {
            QueueFamilyIndices.graphics = graphicsQueueIndex;
        }
        QueueFamilyIndices.present = presentQueueIndex;
    }

    if ((QueueFamilyIndices.present != QueueFamilyIndices.graphics) &&
        (QueueFamilyIndices.present != QueueFamilyIndices.compute ) &&
        (QueueFamilyIndices.present != QueueFamilyIndices.transfer)) {
        queueCreateInfos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = QueueFamilyIndices.present,
            .queueCount = 1,
            .pQueuePriorities = &defaultQueuePriority,
        });
    }

    auto it = std::find_if(enabledExtensionNames.begin(), enabledExtensionNames.end(), [](const char* enabledExtensionName){
        return strcmp(enabledExtensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
    });

    if(it == enabledExtensionNames.end()){
        enabledExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    EnabledLayerNames = enabledLayerNames;
    EnabledExtensionNames = enabledExtensionNames;
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

    VK_ASSERT_SUCCESSED(vkCreateDevice(physicalDeviceInfo.PhysicalDevice, &deviceCI, nullptr, &Device));

    // get device queue
    std::vector<uint32_t> queueFamilyList{
        QueueFamilyIndices.graphics,
        QueueFamilyIndices.compute,
        QueueFamilyIndices.transfer,
        QueueFamilyIndices.present
    };
    std::vector<VkQueue *> queues{GraphicsQueue.AddressOf(), ComputeQueue.AddressOf(), TransferQueue.AddressOf(), PresentQueue.AddressOf()};
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
                vkGetDeviceQueue(Device, queueFamilyList[i], 0, queues[i]);
            }
        }
    }
}

void VulkanDevice::Destroy() 
{
    if(Device != VK_NULL_HANDLE){
        vkDestroyDevice(Device,nullptr);
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

VkImageView VulkanDevice::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageView imageView;
    VkImageViewCreateInfo imageViewCI{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                      .image = image,
                                      .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                      .format = format,
                                      .components = {},
                                      .subresourceRange{
                                          .aspectMask = aspectFlags,
                                          .baseMipLevel = 0,
                                          .levelCount = 1,
                                          .baseArrayLayer = 0,
                                          .layerCount = 1,
                                      }};
    VK_ASSERT_SUCCESSED(vkCreateImageView(Device, &imageViewCI, nullptr, &imageView));
    return imageView;
}