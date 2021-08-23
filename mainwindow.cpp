#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>

#include "Widget/VulkanInstanceConfigWidget.h"
#include "Widget/VulkanDeviceConfigWidget.h"


MainWindow* GMainWindow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

    //, ui(new Ui::MainWindow)
{
    _pGraphics = std::make_unique<VulkanGraphics>();
    //ui->setupUi(this);
    _pVulkanInstanceConfigWidget = new VulkanInstanceConfigWidget(this);
    setCentralWidget(_pVulkanInstanceConfigWidget);
    //setSurfaceType(Surface::VulkanSurface);
}

MainWindow::~MainWindow()
{
    _pGraphics.reset();
    //delete ui;
}

void MainWindow::CreateVulkanInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    _pGraphics->CreateInstance(enabledInstanceLayers, enabledInstanceExtensions);
    _pVulkanDeviceConfigWidget = new VulkanDeviceConfigWidget(this);
    setCentralWidget(_pVulkanDeviceConfigWidget);
    _pVulkanInstanceConfigWidget->deleteLater();
}

void MainWindow::CreateVulkanDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    VkSurfaceKHR surface;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR surfaceCI{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = (HINSTANCE)::GetModuleHandle(NULL),
        .hwnd = (HWND)winId(),
    };
    VK_THROW_EXCEPT(vkCreateWin32SurfaceKHR(*(_pGraphics->_Instance), &surfaceCI, nullptr, &surface));
#elif defined (VK_USE_PLATFORM_MACOS_MVK)
    VkMacOSSurfaceCreateInfoMVK surfaceCI{
        .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .flags = 0,
        .pView = (void*) winId(),
    };
    VK_THROW_EXCEPT(vkCreateMacOSSurfaceMVK(*(_pGraphics->_Instance), &surfaceCI, nullptr, &surface));
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VK_THROW_EXCEPT(vkCreateAndroidSurfaceKHR();
#endif
    _pGraphics->CreateDevice(physicalDeviceInfo, surface, enabledLayerNames, enabledExtensionNames, enabledFeatures);
    _pVulkanDeviceConfigWidget->deleteLater();
}
