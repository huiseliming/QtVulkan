#pragma once
#include <VulkanTools.h>
#include <vector>
#include <vulkan/vulkan_core.h>

struct VulkanDevice;
struct SwapChainSupportDetails;

struct VulkanSwapchain{
    VulkanSwapchain() = delete;
    VulkanSwapchain(VulkanDevice& device);
    //VulkanDevice(std::vector<const char*> enabledInstanceLayers, std::vector<const char*> enabledInstanceExtensions);
    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain(VulkanSwapchain&&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

    ~VulkanSwapchain();

    operator VkSwapchainKHR() const { return Swapchain; }
    
    void Create(VkSurfaceKHR surface, SwapChainSupportDetails swapchainSupport, VkExtent2D windowExtent2D);

    void Destroy();

    void CreateImageViews();

    void DestroyImageViews();

    uint32_t SwapchainMinImageCount;
    VkFormat SwapchainImageFormat;
    VkExtent2D SwapchainExtent;

    uint32_t SwapchainImageCount;
    std::vector<VkImage> SwapchainImages;
    std::vector<VkImageView> SwapchainImageViews;

    VkSwapchainKHR Swapchain;
    VulkanDevice& Device;
};
