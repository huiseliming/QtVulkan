#pragma once

#include "VulkanInstance.h"
#include "VulkanQueue.h"
#include <algorithm>
#include <memory>


struct VulkanGraphics
{
    ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice();

    std::unique_ptr<VulkanInstance> _Instance;
    std::unique_ptr<VulkanDevice> _Device;
    VulkanQueue _GraphicsQueue;
    VulkanQueue _ComputeQueue;
    VulkanQueue _TransferQueue;
    VulkanQueue _PresentQueue;

};

