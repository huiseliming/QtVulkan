#include <VulkanGraphics.h>
#include <functional>
#include <memory>
#include <vulkan/vulkan_core.h>
#include <cassert>

VulkanGraphics::~VulkanGraphics()
{
    pDevice.reset();
    pSurface.reset();
    pInstance.reset();
}

void VulkanGraphics::CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    pInstance = std::make_unique<VulkanInstance>(enabledInstanceLayers, enabledInstanceExtensions);
}

void VulkanGraphics::CreateDevice(VulkanPhysicalDeviceInfo &physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char *>& enabledLayerNames, std::vector<const char *>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(surface != VK_NULL_HANDLE);
    pSurface = std::move(std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>>
        (new VkSurfaceKHR{surface},[this](VkSurfaceKHR* pSurface) {
            vkDestroySurfaceKHR(*pInstance, *pSurface, nullptr);
            delete pSurface;
        }));
    pDevice = std::make_unique<VulkanDevice>();
    pDevice->CreateDevice(physicalDeviceInfo, surface, enabledLayerNames, enabledExtensionNames, enabledFeatures);
}

void VulkanGraphics::CreateSwapchain()
{
    SwapChainSupportDetails swapchainSupport = QuerySwapchainSupport();
    VkSurfaceFormatKHR surfaceFormat = GetAvailableFormat(swapchainSupport.SurfaceFormats);
    VkPresentModeKHR presentMode = GetAvailablePresentMode(swapchainSupport.PresentModes);
    VkExtent2D extent = GetAvailableExtent(swapchainSupport.SurfaceCapabilities);
    uint32_t swapchainMinImageCount_ = swapchainSupport.SurfaceCapabilities.minImageCount + 1;
    if (swapchainSupport.SurfaceCapabilities.maxImageCount > 0 &&
        swapchainMinImageCount_ > swapchainSupport.SurfaceCapabilities.maxImageCount)
        swapchainMinImageCount_ = swapchainSupport.SurfaceCapabilities.maxImageCount;
    VkSwapchainKHR oldSwapchainKHR = *pSwapchain;
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
        .surface = *pSurface,
        .minImageCount = swapchainMinImageCount_,
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
    uint32_t queueFamilyIndices[] = {pDevice->QueueFamilyIndices.graphics, pDevice->QueueFamilyIndices.present};
    if (pDevice->QueueFamilyIndices.graphics != pDevice->QueueFamilyIndices.present) {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // VK_ASSERT_SUCCESSED(vkCreateSwapchainKHR(_Device, &swapchainCI, nullptr, &swapchainKHR_));
    // if (oldSwapchainKHR != VK_NULL_HANDLE) {
    //     DestroySwapchainImageViews();
    //     vkDestroySwapchainKHR(device_, oldSwapchainKHR, nullptr);
    // }
    // swapchainImageFormat_ = surfaceFormat.format;
    // swapchainExtent_ = extent;
    // vkGetSwapchainImagesKHR(device_, swapchainKHR_, &swapchainImageCount_, nullptr);
    // swapchainImages_.resize(swapchainImageCount_);
    // vkGetSwapchainImagesKHR(device_, swapchainKHR_, &swapchainImageCount_, swapchainImages_.data());
    // CreateSwapchainImageViews();
}

SwapChainSupportDetails VulkanGraphics::QuerySwapchainSupport()
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice->PhysicalDevice, *pSurface, &details.SurfaceCapabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice->PhysicalDevice, *pSurface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.SurfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice->PhysicalDevice, *pSurface, &formatCount, details.SurfaceFormats.data());
    }
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice->PhysicalDevice, *pSurface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.PresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice->PhysicalDevice, *pSurface, &presentModeCount, details.PresentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR VulkanGraphics::GetAvailableFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    return availableFormats[0];
}

VkPresentModeKHR VulkanGraphics::GetAvailablePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanGraphics::GetAvailableExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        // glfwGetFramebufferSize(pWindow_->pGLFWWindow(), &width, &height);
        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        actualExtent.width =
            std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height =
            std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}