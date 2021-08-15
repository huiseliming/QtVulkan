#ifndef VULKANCONFIGWIDGET_H
#define VULKANCONFIGWIDGET_H

#include <QWidget>
#include <VulkanGraphics.h>
#include <mainwindow.h>
#include <QCheckBox>

class VulkanConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VulkanConfigWidget(QWidget *parent = nullptr);
    ~VulkanConfigWidget();

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
    std::vector<bool> _InstanceLayerStates;
    std::vector<bool> _InstanceExtensionStates;
    std::vector<VkLayerProperties> _InstanceLayerProperties;
    std::vector<VkExtensionProperties> _InstanceExtensionProperties;
    MainWindow* _pMainWindow{nullptr};

signals:

};

#endif // VULKANCONFIGWIDGET_H
