#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanTools.h"
#include <memory>
#include <vector>
#include <set>
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

void VulkanDevice::CreateDevice(std::vector<VkDeviceQueueCreateInfo> queueCreateInfos, std::vector<const char*> enabledLayerNames, std::vector<const char*> enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
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
