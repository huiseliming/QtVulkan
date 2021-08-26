#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include "VulkanImageResource.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <functional>
#include <vulkan/vulkan_core.h>

class VulkanWindow;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;

    VkSurfaceFormatKHR GetAvailableFormat()
    {
        for (const auto &availableFormat : SurfaceFormats)
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        return SurfaceFormats[0];
    }

    VkPresentModeKHR GetAvailablePresentMode()
    {
        for (const auto &availablePresentMode : PresentModes)
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D GetAvailableExtent(VkExtent2D windowExtent)
    {
        if (SurfaceCapabilities.currentExtent.width != UINT32_MAX) {
            return SurfaceCapabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width =
                std::max(SurfaceCapabilities.minImageExtent.width, std::min(SurfaceCapabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height =
                std::max(SurfaceCapabilities.minImageExtent.height, std::min(SurfaceCapabilities.maxImageExtent.height, actualExtent.height));
            return actualExtent;
        }
    }
};

struct VulkanGraphics
{
    virtual ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateGraphicsWindow();
    void CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures);
    void CreateSwapchain();
    
    void Initialize();
    void Cleanup();

    std::unique_ptr<VulkanInstance> pInstance;
    std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>> pSurface;
    std::unique_ptr<VulkanDevice> pDevice;
    std::unique_ptr<VulkanSwapchain> pSwapchain;
    std::unique_ptr<VulkanImageResource> pDepthResources;
    
    std::unique_ptr<VulkanWindow> pGraphicsWindow;

    const VulkanQueue& GraphicsQueue() { return pDevice->GraphicsQueue; }
    const VulkanQueue& ComputeQueue()  { return pDevice->ComputeQueue; }
    const VulkanQueue& TransferQueue() { return pDevice->TransferQueue; }
    const VulkanQueue& PresentQueue()  { return pDevice->PresentQueue; }

    SwapChainSupportDetails QuerySwapchainSupport();
};

