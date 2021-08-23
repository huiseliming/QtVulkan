#include <VulkanGraphics.h>

VulkanGraphics::~VulkanGraphics()
{
    _Device.reset();
    _Instance.reset();
}

void VulkanGraphics::CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    _Instance = std::make_unique<VulkanInstance>(enabledInstanceLayers, enabledInstanceExtensions);
}

void VulkanGraphics::CreateDevice(VulkanPhysicalDeviceInfo &physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char *>& enabledLayerNames, std::vector<const char *>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    _Device = std::make_unique<VulkanDevice>();
    _Device->CreateDevice(physicalDeviceInfo, surface, enabledLayerNames, enabledExtensionNames, enabledFeatures);
}
