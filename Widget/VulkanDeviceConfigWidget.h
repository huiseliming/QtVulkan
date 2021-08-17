#ifndef VULKANDEVICECONFIGWIDGET_H
#define VULKANDEVICECONFIGWIDGET_H

#include <QObject>
#include <QWidget>
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
    int32_t _SelectedIndex;
    std::vector<const char*> _extensionNames;
    MainWindow* _pMainWindow{nullptr};
signals:

};

#endif // VULKANDEVICECONFIGWIDGET_H
