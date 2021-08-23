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
    QListWidget* _pExtensionListWidget{nullptr};
    QTextBrowser* _pTextBrowser{nullptr};

    std::vector<VkPhysicalDevice> _PhysicalDevices;
    std::vector<VulkanPhysicalDeviceInfo> _PhysicalDeviceInfos;

    uint32_t _SelectedDeviceIndex = UINT32_MAX;
    std::vector<const char*> _LayerNames;
    std::vector<const char*> _ExtensionNames;
    MainWindow* _pMainWindow{nullptr};
    
signals:

};

#endif // VULKANDEVICECONFIGWIDGET_H
