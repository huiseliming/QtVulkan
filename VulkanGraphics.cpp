#include <VulkanGraphics.h>
#include <cstdio>
#include <functional>
#include <memory>
#include <vulkan/vulkan_core.h>
#include <cassert>
#include "VulkanWindow.h"

VulkanGraphics::~VulkanGraphics()
{
    Cleanup();
    pDevice.reset();
    pSurface.reset();
    pGraphicsWindow.reset();
    pInstance.reset();
}

void VulkanGraphics::CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    assert(pInstance.get() == nullptr);
    pInstance = std::make_unique<VulkanInstance>(enabledInstanceLayers, enabledInstanceExtensions);
}

void VulkanGraphics::CreateGraphicsWindow()
{
    assert(pGraphicsWindow.get() == nullptr);
    pGraphicsWindow = std::make_unique<VulkanWindow>();
}

void VulkanGraphics::CreateDevice(VulkanPhysicalDeviceInfo &physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char *>& enabledLayerNames, std::vector<const char *>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(pDevice.get() == nullptr);
    assert(surface != VK_NULL_HANDLE);
    pSurface = std::move(std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>>
        (new VkSurfaceKHR{surface},[this](VkSurfaceKHR* pSurface) {
            vkDestroySurfaceKHR(*pInstance, *pSurface, nullptr);
            delete pSurface;
        }));
    pDevice = std::make_unique<VulkanDevice>();
    pDevice->Create(physicalDeviceInfo, surface, enabledLayerNames, enabledExtensionNames, enabledFeatures);
}

void VulkanGraphics::Initialize()
{
    pSwapchain = std::make_unique<VulkanSwapchain>(*pDevice);
    pSwapchain->Create(*pSurface, QuerySwapchainSupport(), pGraphicsWindow->GetWindowExtent2D());
}

void VulkanGraphics::Cleanup()
{
    pSwapchain.reset();
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
