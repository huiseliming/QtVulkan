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
    std::vector<QCheckBox*> _InstanceLayerCheckBoxs;
    std::vector<QCheckBox*> _InstanceExtensionCheckBoxs;
    std::vector<VkLayerProperties> _InstanceLayerProperties;
    std::vector<VkExtensionProperties> _InstanceExtensionProperties;
    MainWindow* _pMainWindow{nullptr};

signals:

};

#endif // VULKANCONFIGWIDGET_H
