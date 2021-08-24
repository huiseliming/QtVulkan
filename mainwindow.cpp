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
    pGraphics = std::make_unique<VulkanGraphics>();
    setMinimumSize(800,600);
    //ui->setupUi(this);
    pVulkanInstanceConfigWidget = new VulkanInstanceConfigWidget(this);
    setCentralWidget(pVulkanInstanceConfigWidget);
    //setSurfaceType(Surface::VulkanSurface);
}

MainWindow::~MainWindow()
{
    pGraphics.reset();
    //delete ui;
}

void MainWindow::CreateVulkanInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions)
{
    pGraphics->CreateInstance(enabledInstanceLayers, enabledInstanceExtensions);
    pVulkanDeviceConfigWidget = new VulkanDeviceConfigWidget(this);
    setCentralWidget(pVulkanDeviceConfigWidget);
    pVulkanInstanceConfigWidget->deleteLater();
}

void MainWindow::CreateVulkanDevice(VulkanPhysicalDeviceInfo& physicalDeviceInfo, std::vector<const char*>& enabledLayerNames, std::vector<const char*>& enabledExtensionNames, VkPhysicalDeviceFeatures enabledFeatures)
{
    pVulkanWindow = std::make_unique<VulkanWindow>();
     pGraphics->CreateDevice(physicalDeviceInfo, pVulkanWindow->CreateSurfaceKHR(*pGraphics->pInstance), enabledLayerNames, enabledExtensionNames, enabledFeatures);
    QWidget *container = QWidget::createWindowContainer(pVulkanWindow.get());
    setCentralWidget(container);
    pVulkanDeviceConfigWidget->deleteLater();
}
