#include <VulkanGraphics.h>
#include <functional>
#include <memory>
#include <vulkan/vulkan_core.h>

VulkanGraphics::~VulkanGraphics()
{
    _Device.reset();
    _Surface.reset();
    _Instance.reset();
}

void VulkanGraphics::CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    _Instance = std::make_unique<VulkanInstance>(enabledInstanceLayers, enabledInstanceExtensions);
}

void VulkanGraphics::CreateDevice(VulkanPhysicalDeviceInfo &physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char *>& enabledLayerNames, std::vector<const char *>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    assert(surface != VK_NULL_HANDLE);
    _Surface = std::move(std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>>
        (new VkSurfaceKHR{surface},[this](VkSurfaceKHR* pSurface) {
            vkDestroySurfaceKHR(*_Instance, *pSurface, nullptr);
            delete pSurface;
        }));
    _Device = std::make_unique<VulkanDevice>();
    _Device->CreateDevice(physicalDeviceInfo, surface, enabledLayerNames, enabledExtensionNames, enabledFeatures);
}
