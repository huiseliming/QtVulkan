#pragma once

#include <QWindow>
#include <vulkan/vulkan_core.h>
#include "VulkanTools.h"

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow(QWindow* parent = nullptr);
    ~VulkanWindow();

    void DestroySurface();

    VkSurfaceKHR GetSurfaceKHR(VkInstance instance);
    VkInstance _Instance{VK_NULL_HANDLE};
    VkSurfaceKHR _Surface{VK_NULL_HANDLE};
};



