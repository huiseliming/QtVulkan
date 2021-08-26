#pragma once
#include <functional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "VulkanTools.h"
#include "VulkanInstance.h"
#include <vector>
#include <optional>

#include "VulkanQueue.h"

struct VulkanPhysicalDeviceInfo
{
    VulkanPhysicalDeviceInfo() = delete;
    VulkanPhysicalDeviceInfo(VkPhysicalDevice physicalDevice);
    VulkanPhysicalDeviceInfo(const VulkanPhysicalDeviceInfo&) = default;
    VulkanPhysicalDeviceInfo(VulkanPhysicalDeviceInfo&&);
    VulkanPhysicalDeviceInfo& operator=(const VulkanPhysicalDeviceInfo&) = default;
    VulkanPhysicalDeviceInfo &operator=(VulkanPhysicalDeviceInfo&&);

    uint64_t GetDeviceLocalMemorySize();

    VkPhysicalDevice PhysicalDevice{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties PhysicalDeviceProperties;
    VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
    std::vector<VkExtensionProperties> SupportedExtensionProperties;
    std::vector<VkLayerProperties> SupportedLayerProperties;
};

struct VulkanDevice
{
    VulkanDevice() = default;
    //VulkanDevice(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions);
    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;
    virtual ~VulkanDevice();

    operator VkDevice() const { return Device; }

    struct {
        uint32_t graphics{UINT32_MAX};
        uint32_t compute{UINT32_MAX};
        uint32_t transfer{UINT32_MAX};
        uint32_t present{UINT32_MAX};
    } QueueFamilyIndices;

    VulkanQueue GraphicsQueue;
    VulkanQueue ComputeQueue;
    VulkanQueue TransferQueue;
    VulkanQueue PresentQueue;

    void Create(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*> enabledLayerNames ={}, std::vector<const char*> enabledExtensionNames = {}, VkPhysicalDeviceFeatures enabledFeatures = {});
    void Destroy();



public:
    static std::vector<VkPhysicalDevice> GetPhysicalDevices(VulkanInstance& instance);

public:
    std::vector<const char*> EnabledExtensionNames;
    std::vector<const char*> EnabledLayerNames;
    VkPhysicalDevice PhysicalDevice{VK_NULL_HANDLE};
    VkDevice Device{VK_NULL_HANDLE};

    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

};


