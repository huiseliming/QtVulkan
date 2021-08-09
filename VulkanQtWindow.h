#pragma once

#include <QWindow>


class VulkanQtWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanQtWindow(QWindow* parent = nullptr);
    ~VulkanQtWindow();
    
};



