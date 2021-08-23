#pragma once

#include <QWindow>
#include "VulkanTools.h"

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow(QWindow* parent = nullptr);
    ~VulkanWindow();

    VkSurfaceKHR GetSurfaceKHR(VkInstance instance);
    VkSurfaceKHR _Surface{VK_NULL_HANDLE};
};



