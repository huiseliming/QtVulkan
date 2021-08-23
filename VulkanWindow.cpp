#include "VulkanWindow.h"
#include <vulkan/vulkan_core.h>

VulkanWindow::VulkanWindow(QWindow* parent) 
{
    setSurfaceType(QSurface::VulkanSurface);
}

VulkanWindow::~VulkanWindow() 
{
    DestroySurface();
}


void VulkanWindow::DestroySurface()
{
    if(_Surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(_Instance, _Surface, nullptr);
        _Surface = VK_NULL_HANDLE;
        _Instance = VK_NULL_HANDLE;
    }
}

VkSurfaceKHR VulkanWindow::GetSurfaceKHR(VkInstance instance)
{
    assert(instance != VK_NULL_HANDLE);
    if(_Instance != instance) {
        DestroySurface();
    }
    if (_Surface == VK_NULL_HANDLE) {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        VkWin32SurfaceCreateInfoKHR surfaceCI{
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .hinstance = (HINSTANCE)::GetModuleHandle(NULL),
            .hwnd = (HWND) winId(),
        };
        VK_ASSERT_SUCCESSED(vkCreateWin32SurfaceKHR(instance, &surfaceCI, nullptr, &_Surface));
#elif defined (VK_USE_PLATFORM_MACOS_MVK)
        VkMacOSSurfaceCreateInfoMVK surfaceCI{
            .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
            .pNext = nullptr,
            .flags = 0,
            .pView = (void*) winId(),
        };
        VK_ASSERT_SUCCESSED(vkCreateMacOSSurfaceMVK(instance, &surfaceCI, nullptr, &_Surface));
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        VK_ASSERT_SUCCESSED(vkCreateAndroidSurfaceKHR();
#endif
        _Instance = instance;
    }
    return _Surface;
}
    

