#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <functional>
#include <vulkan/vulkan_core.h>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR _SurfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> _SurfaceFormats;
    std::vector<VkPresentModeKHR> _PresentModes;
};

struct VulkanGraphics
{
    virtual ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures);
    void CreateSwapchain();

    std::unique_ptr<VulkanInstance> _Instance;
    std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>> _Surface;
    std::unique_ptr<VulkanDevice> _Device;
    std::unique_ptr<VulkanSwapchain> _Swapchain;

    const VulkanQueue& GraphicsQueue() { return _Device->_GraphicsQueue; }
    const VulkanQueue& ComputeQueue()  { return _Device->_ComputeQueue; }
    const VulkanQueue& TransferQueue() { return _Device->_TransferQueue; }
    const VulkanQueue& PresentQueue()  { return _Device->_PresentQueue; }

    SwapChainSupportDetails QuerySwapchainSupport();
    VkSurfaceFormatKHR GetAvailableFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR GetAvailablePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D GetAvailableExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};

