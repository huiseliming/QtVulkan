#pragma once
#include <functional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VulkanTools.h"
#include "VulkanInstance.h"
#include <vector>

struct VulkanPhysicalDevices
{
protected:
    VulkanPhysicalDevices() = default;
public:
    VulkanPhysicalDevices(const VulkanPhysicalDevices&) = delete;
    VulkanPhysicalDevices(VulkanPhysicalDevices&&);
    VulkanPhysicalDevices& operator=(const VulkanPhysicalDevices&) = delete;
    VulkanPhysicalDevices& operator=(VulkanPhysicalDevices&&);

    static VulkanPhysicalDevices Get(VulkanInstance& instance);

    std::vector<VkPhysicalDevice> GetPhysicalDevicesSuitable(std::function<bool(VkPhysicalDevice&)> physicalDevicesSuitable);
    std::vector<VkPhysicalDevice> _PhysicalDevices;
};


struct VulkanDevice
{
    
    VkDevice _Device;
};


