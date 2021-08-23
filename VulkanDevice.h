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
    std::optional<uint32_t> GetGraphicsQueueIndex();
    std::optional<uint32_t> GetComputeQueueIndex();
    std::optional<uint32_t> GetTransferQueueIndex();


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

    struct {
        uint32_t graphics{UINT32_MAX};
        uint32_t compute{UINT32_MAX};
        uint32_t transfer{UINT32_MAX};
        uint32_t present{UINT32_MAX};
    } _QueueFamilyIndices;

    VulkanQueue _GraphicsQueue;
    VulkanQueue _ComputeQueue;
    VulkanQueue _TransferQueue;
    VulkanQueue _PresentQueue;

    void CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*> enabledLayerNames ={}, std::vector<const char*> enabledExtensionNames = {}, VkPhysicalDeviceFeatures enabledFeatures = {});
    void DestroyDevice();

public:
    static std::vector<VkPhysicalDevice> GetPhysicalDevices(VulkanInstance& instance);

public:
    VkPhysicalDevice _PhysicalDevice;
    VkDevice _Device;

};


