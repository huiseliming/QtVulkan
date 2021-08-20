#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include <algorithm>
#include <memory>


struct VulkanGraphics
{
    virtual ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice();

    std::unique_ptr<VulkanInstance> _Instance;
    std::unique_ptr<VulkanDevice> _Device;

    VulkanQueue& GraphicsQueue() { return _Device->_GraphicsQueue; }
    VulkanQueue& ComputeQueue()  { return _Device->_ComputeQueue; }
    VulkanQueue& TransferQueue() { return _Device->_TransferQueue; }
    VulkanQueue& PresentQueue()  { return _Device->_PresentQueue; }

};

