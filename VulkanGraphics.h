#pragma once

#include "VulkanInstance.h"
#include <algorithm>
#include <memory>

struct VulkanGraphics
{
    ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice();

    std::unique_ptr<VulkanInstance> _Instance;
};
