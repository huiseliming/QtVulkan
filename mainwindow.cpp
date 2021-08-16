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
