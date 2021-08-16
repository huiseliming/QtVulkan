#pragma once
#include <functional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VulkanTools.h"
#include "VulkanInstance.h"
#include <vector>


struct VulkanPhysicalDeviceInfo
{
    VulkanPhysicalDeviceInfo() = delete;
    VulkanPhysicalDeviceInfo(VkPhysicalDevice physicalDevice);
    VulkanPhysicalDeviceInfo(const VulkanPhysicalDeviceInfo&) = default;
    VulkanPhysicalDeviceInfo(VulkanPhysicalDeviceInfo&&);
    VulkanPhysicalDeviceInfo& operator=(const VulkanPhysicalDeviceInfo&) = default;
    VulkanPhysicalDeviceInfo &operator=(VulkanPhysicalDeviceInfo&&);

    VkPhysicalDevice _PhysicalDevice;
    VkPhysicalDeviceProperties _PhysicalDeviceProperties;
    VkPhysicalDeviceFeatures _PhysicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties _PhysicalDeviceMemoryProperties;
    std::vector<VkQueueFamilyProperties> _QueueFamilyProperties;
    std::vector<VkExtensionProperties> _SupportedExtensionProperties;
};

struct VulkanDevice
{
    static std::vector<VkPhysicalDevice> GetPhysicalDevices(VulkanInstance& instance);
    VkPhysicalDevice _PhysicalDevice;
    VkDevice _Device;
};


