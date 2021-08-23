#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <functional>
#include <vulkan/vulkan_core.h>


struct VulkanGraphics
{
    virtual ~VulkanGraphics();

    void CreateInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);
    void CreateDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, VkSurfaceKHR surface, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures);

    std::unique_ptr<VulkanInstance> _Instance;
    std::unique_ptr<VkSurfaceKHR, std::function<void (VkSurfaceKHR*)>> _Surface;
    std::unique_ptr<VulkanDevice> _Device;

    const VulkanQueue& GraphicsQueue() { return _Device->_GraphicsQueue; }
    const VulkanQueue& ComputeQueue()  { return _Device->_ComputeQueue; }
    const VulkanQueue& TransferQueue() { return _Device->_TransferQueue; }
    const VulkanQueue& PresentQueue()  { return _Device->_PresentQueue; }

};

