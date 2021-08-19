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
