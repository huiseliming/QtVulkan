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

    uint64_t GetDeviceLocalMemorySize();

    VkPhysicalDevice _PhysicalDevice;
    VkPhysicalDeviceProperties _PhysicalDeviceProperties;
    VkPhysicalDeviceFeatures _PhysicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties _PhysicalDeviceMemoryProperties;
    std::vector<VkQueueFamilyProperties> _QueueFamilyProperties;
    std::vector<VkExtensionProperties> _SupportedExtensionProperties;
};

struct VulkanDevice
{
    VulkanDevice() = default;
    //VulkanDevice(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions);
    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;

    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;



private:
    void CreateDevice(std::vector<VkDeviceQueueCreateInfo> queueCreateInfos, std::vector<const char*> enabledLayerNames, std::vector<const char*> enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures);
    void DestroyDevice();

public:
    static std::vector<VkPhysicalDevice> GetPhysicalDevices(VulkanInstance& instance);

public:
    VkPhysicalDevice _PhysicalDevice;
    VkDevice _Device;

};


