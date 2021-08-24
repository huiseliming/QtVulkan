#include "VulkanSwapchain.h"
#include "VulkanGraphics.h"
#include <cassert>
#include <vulkan/vulkan_core.h>

VulkanSwapchain::VulkanSwapchain(VulkanDevice& device)
    :Device(device)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
    Destroy();
}

void VulkanSwapchain::Create(VkSurfaceKHR surface, SwapChainSupportDetails swapchainSupport, VkExtent2D windowExtent2D)
{
    VkSurfaceFormatKHR surfaceFormat = swapchainSupport.GetAvailableFormat();
    VkPresentModeKHR presentMode = swapchainSupport.GetAvailablePresentMode();
    VkExtent2D extent = swapchainSupport.GetAvailableExtent(windowExtent2D);
    SwapchainMinImageCount = swapchainSupport.SurfaceCapabilities.minImageCount + 1;
    if (swapchainSupport.SurfaceCapabilities.maxImageCount > 0 &&
        SwapchainMinImageCount > swapchainSupport.SurfaceCapabilities.maxImageCount)
        SwapchainMinImageCount = swapchainSupport.SurfaceCapabilities.maxImageCount;
    VkSwapchainKHR oldSwapchainKHR = Swapchain;
    VkSurfaceTransformFlagBitsKHR preTranform{};

    if ((swapchainSupport.SurfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) != 0) {
        preTranform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        preTranform = swapchainSupport.SurfaceCapabilities.currentTransform;
    }

    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };

    if (std::find(compositeAlphaFlags.begin(), compositeAlphaFlags.end(), VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) !=
        compositeAlphaFlags.end()) {
        compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    } else {
        // TODO: print warn
        for (auto &compositeAlphaFlag : compositeAlphaFlags) {
            if (swapchainSupport.SurfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag) {
                compositeAlpha = compositeAlphaFlag;
                break;
            };
        }
    }

    VkSwapchainCreateInfoKHR swapchainCI{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = SwapchainMinImageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = preTranform,
        .compositeAlpha = compositeAlpha,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = oldSwapchainKHR,
    };
    uint32_t queueFamilyIndices[] = {Device.QueueFamilyIndices.graphics, Device.QueueFamilyIndices.present};
    if (Device.QueueFamilyIndices.graphics != Device.QueueFamilyIndices.present) {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    VK_ASSERT_SUCCESSED(vkCreateSwapchainKHR(Device, &swapchainCI, nullptr, &Swapchain));
    if (oldSwapchainKHR != VK_NULL_HANDLE) {
        DestroyImageViews();
        vkDestroySwapchainKHR(Device, oldSwapchainKHR, nullptr);
    }
    SwapchainImageFormat = surfaceFormat.format;
    SwapchainExtent = extent;
    vkGetSwapchainImagesKHR(Device, Swapchain, &SwapchainImageCount, nullptr);
    SwapchainImages.resize(SwapchainImageCount);
    vkGetSwapchainImagesKHR(Device, Swapchain, &SwapchainImageCount, SwapchainImages.data());
    CreateImageViews();
}

void VulkanSwapchain::Destroy()
{
    if (Swapchain != VK_NULL_HANDLE) {
        DestroyImageViews();
        vkDestroySwapchainKHR(Device, Swapchain, nullptr);
        Swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::CreateImageViews()
{
    assert(SwapchainImageViews.empty());
    SwapchainImageViews.resize(SwapchainImageCount);
    for (uint32_t i =0; i < SwapchainImageViews.size(); i++) {
        VkImageViewCreateInfo imageViewCI{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = SwapchainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = SwapchainImageFormat,
            .components = {},
            .subresourceRange{
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };
        VK_ASSERT_SUCCESSED(vkCreateImageView(Device, &imageViewCI, nullptr, &SwapchainImageViews[i]));
    }
}

void VulkanSwapchain::DestroyImageViews()
{
    for (uint32_t i =0; i < SwapchainImageViews.size(); i++) {
        vkDestroyImageView(Device, SwapchainImageViews[i], nullptr);
    }
    SwapchainImageViews.clear();
}
