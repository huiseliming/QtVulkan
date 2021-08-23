#include "VulkanWindow.h"
#include <vulkan/vulkan_core.h>

VulkanWindow::VulkanWindow(QWindow* parent) 
{
    setSurfaceType(QSurface::VulkanSurface);
}

VulkanWindow::~VulkanWindow() 
{
}

VkSurfaceKHR VulkanWindow::CreateSurfaceKHR(VkInstance instance)
{
    assert(instance != VK_NULL_HANDLE);
    VkSurfaceKHR surface;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        VkWin32SurfaceCreateInfoKHR surfaceCI{
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .hinstance = (HINSTANCE)::GetModuleHandle(NULL),
            .hwnd = (HWND) winId(),
        };
        VK_ASSERT_SUCCESSED(vkCreateWin32SurfaceKHR(instance, &surfaceCI, nullptr, &surface));
#elif defined (VK_USE_PLATFORM_MACOS_MVK)
        VkMacOSSurfaceCreateInfoMVK surfaceCI{
            .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
            .pNext = nullptr,
            .flags = 0,
            .pView = (void*) winId(),
        };
        VK_ASSERT_SUCCESSED(vkCreateMacOSSurfaceMVK(instance, &surfaceCI, nullptr, &surface));
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        VK_ASSERT_SUCCESSED(vkCreateAndroidSurfaceKHR();
#endif
    return surface;
}
    

