#ifndef VULKANCONFIGWIDGET_H
#define VULKANCONFIGWIDGET_H

#include <QWidget>
#include <VulkanGraphics.h>
#include <mainwindow.h>
#include <QCheckBox>

class VulkanInstanceConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VulkanInstanceConfigWidget(QWidget *parent = nullptr);
    ~VulkanInstanceConfigWidget();

    void CreateVulkanInstanceLyaerAndExtensionSelector();
    void DefaultVulkanInstanceLyaerAndExtensionConfig();
    void SaveVulkanInstanceLyaerAndExtensionConfig();
    void LoadVulkanInstanceLyaerAndExtensionConfig();

public slots:
    void OnCreateInstanceButtonClicked();
    void OnDefaultSettingsButtonClicked();
public:
    std::vector<QCheckBox*> InstanceLayerCheckBoxs;
    std::vector<QCheckBox*> InstanceExtensionCheckBoxs;
    std::vector<VkLayerProperties> InstanceLayerProperties;
    std::vector<VkExtensionProperties> InstanceExtensionProperties;
    MainWindow* pMainWindow{nullptr};

signals:

};

#endif // VULKANCONFIGWIDGET_H
