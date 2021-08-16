#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include <memory>
#include <vector>
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

std::vector<VkPhysicalDevice> VulkanDevice::GetPhysicalDevices(VulkanInstance& instance)
{
    std::vector<VkPhysicalDevice> physicalDevices;
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    physicalDevices.resize(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
    return physicalDevices;
}
