#ifndef VULKANDEVICECONFIGWIDGET_H
#define VULKANDEVICECONFIGWIDGET_H

#include <QObject>
#include <QWidget>
#include <cstdint>
#include <vector>
#include "VulkanDevice.h"
#include "mainwindow.h"

class QListWidget;
class QTextBrowser;

class VulkanDeviceConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VulkanDeviceConfigWidget(QWidget *parent = nullptr);

    void CreateVulkanDeviceSelector();

public slots:
    void OnCreateDeviceButtonClicked();

private:
    QListWidget* pLayerNameListWidget{nullptr};
    QListWidget* pExtensionNameListWidget{nullptr};
    QTextBrowser* pTextBrowser{nullptr};

    std::vector<VkPhysicalDevice> PhysicalDevices;
    std::vector<VulkanPhysicalDeviceInfo> PhysicalDeviceInfos;

    uint32_t SelectedDeviceIndex = UINT32_MAX;
    std::vector<const char*> LayerNames;
    std::vector<const char*> ExtensionNames;
    MainWindow* pMainWindow{nullptr};

signals:

};

#endif // VULKANDEVICECONFIGWIDGET_H
