#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

VulkanPhysicalDevices VulkanPhysicalDevices::Get(VulkanInstance& instance) 
{
    VulkanPhysicalDevices physicalDevices;
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    physicalDevices._PhysicalDevices.resize(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices._PhysicalDevices.data());
    return physicalDevices;
}

std::vector<VkPhysicalDevice> VulkanPhysicalDevices::GetPhysicalDevicesSuitable(std::function<bool(VkPhysicalDevice&)> physicalDevicesSuitable) 
{

    std::vector<VkPhysicalDevice> availablePhysicalDevices;
    for (uint32_t i = 0; i < _PhysicalDevices.size(); i++) 
    {
        if(physicalDevicesSuitable(_PhysicalDevices[i]))
        {
            availablePhysicalDevices.push_back(_PhysicalDevices[i]);
        }
    }
    return availablePhysicalDevices;
}

VulkanPhysicalDevices::VulkanPhysicalDevices(VulkanPhysicalDevices&& otherPhysicalDevices) 
{
    if(std::addressof(otherPhysicalDevices) != this)
    {
        this->_PhysicalDevices = std::move(otherPhysicalDevices._PhysicalDevices);
    }
}

VulkanPhysicalDevices& VulkanPhysicalDevices::operator=(VulkanPhysicalDevices&& otherPhysicalDevices) 
{
    if(std::addressof(otherPhysicalDevices) != this)
    {
        this->_PhysicalDevices = std::move(otherPhysicalDevices._PhysicalDevices);
    }
    return *this;
}
