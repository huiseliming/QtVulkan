#pragma once
#include <VulkanTools.h>
#include <vulkan/vulkan_core.h>

struct VulkanSwapchain{

    VulkanSwapchain() = default;
    //VulkanDevice(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions);
    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;
    operator VkSwapchainKHR() const { return _Swapchain; }


    VkSwapchainKHR _Swapchain;
};