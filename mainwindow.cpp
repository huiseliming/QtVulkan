#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <memory>

#include "VulkanWindow.h"
#include "Widget/VulkanInstanceConfigWidget.h"
#include "Widget/VulkanDeviceConfigWidget.h"


MainWindow* GMainWindow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

    //, ui(new Ui::MainWindow)
{
    _pGraphics = std::make_unique<VulkanGraphics>();
    setMinimumSize(800,600);
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
    _pVulkanWindow = std::make_unique<VulkanWindow>();
     _pGraphics->CreateDevice(physicalDeviceInfo, _pVulkanWindow->CreateSurfaceKHR(*_pGraphics->_Instance), enabledLayerNames, enabledExtensionNames, enabledFeatures);
    _pVulkanDeviceConfigWidget->deleteLater();
}
