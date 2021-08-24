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

    VkExtent2D GetWindowExtent2D();

    VkSurfaceKHR CreateSurfaceKHR(VkInstance instance);
};



