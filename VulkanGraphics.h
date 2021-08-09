#pragma once

#include "VulkanInstance.h"
#include <algorithm>
#include <memory>

struct VulkanGraphics
{


    std::unique_ptr<VulkanInstance> _Instance;
};
