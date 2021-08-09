#include "VulkanQtWindow.h"

VulkanQtWindow::VulkanQtWindow(QWindow* parent) 
{
    setSurfaceType(QSurface::VulkanSurface);    
}

VulkanQtWindow::~VulkanQtWindow() 
{
    
}
