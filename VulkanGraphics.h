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
    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;
};

struct VulkanGraphics
{
    virtual ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures);
    void CreateSwapchain();

    std::unique_ptr<VulkanInstance> pInstance;
    std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>> pSurface;
    std::unique_ptr<VulkanDevice> pDevice;
    std::unique_ptr<VulkanSwapchain> pSwapchain;

    const VulkanQueue& GraphicsQueue() { return pDevice->GraphicsQueue; }
    const VulkanQueue& ComputeQueue()  { return pDevice->ComputeQueue; }
    const VulkanQueue& TransferQueue() { return pDevice->TransferQueue; }
    const VulkanQueue& PresentQueue()  { return pDevice->PresentQueue; }

    SwapChainSupportDetails QuerySwapchainSupport();
    VkSurfaceFormatKHR GetAvailableFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR GetAvailablePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D GetAvailableExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};

