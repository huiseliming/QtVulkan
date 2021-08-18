#include "VulkanInstanceConfigWidget.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSettings>

VulkanInstanceConfigWidget::VulkanInstanceConfigWidget(QWidget *parent)
    : QWidget(parent)
    , _pMainWindow(reinterpret_cast<MainWindow*>(parent))
{
    _InstanceLayerProperties = VulkanInstance::EnumerateInstanceLayerProperties();
    _InstanceExtensionProperties = VulkanInstance::EnumerateInstanceExtensionProperties();
    CreateVulkanInstanceLyaerAndExtensionSelector();
    LoadVulkanInstanceLyaerAndExtensionConfig();
}

VulkanInstanceConfigWidget::~VulkanInstanceConfigWidget()
{
    qDebug() << objectName() << "Destroyed!";
}

void VulkanInstanceConfigWidget::CreateVulkanInstanceLyaerAndExtensionSelector()
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout* topHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(topHBoxLayout);

    // list vulkan instance layer
    {
        QListWidget* listWidget = new QListWidget(this);
        for (uint32_t i = 0; i < _InstanceLayerProperties.size(); i++) {
            QCheckBox* checkBox = new QCheckBox(this);
            checkBox->setText(_InstanceLayerProperties[i].layerName);
            QListWidgetItem* listWidgetItem = new QListWidgetItem();
            listWidget->addItem(listWidgetItem);
            listWidget->setItemWidget(listWidgetItem, checkBox);
            _InstanceLayerCheckBoxs.push_back(checkBox);
        }
        topHBoxLayout->addWidget(listWidget);
    }
    // list vulkan instance extension
    {
        QListWidget* listWidget = new QListWidget(this);
        for (uint32_t i = 0; i < _InstanceExtensionProperties.size(); i++) {
            QCheckBox* checkBox = new QCheckBox(this);
            checkBox->setText(_InstanceExtensionProperties[i].extensionName);
            QListWidgetItem* listWidgetItem = new QListWidgetItem();
            listWidget->addItem(listWidgetItem);
            listWidget->setItemWidget(listWidgetItem, checkBox);
            _InstanceExtensionCheckBoxs.push_back(checkBox);
        }
        topHBoxLayout->addWidget(listWidget);
    }

    QHBoxLayout* bottomHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(bottomHBoxLayout);
    // create default settings button
    QPushButton* defaultSettingsPushButton = new QPushButton(this);
    defaultSettingsPushButton->setText("Default Settings");
    QObject::connect(defaultSettingsPushButton, &QPushButton::clicked, this, &VulkanInstanceConfigWidget::OnDefaultSettingsButtonClicked);
    bottomHBoxLayout->addWidget(defaultSettingsPushButton);

    // create create instance button
    QPushButton* createInstancePushButton = new QPushButton(this);
    createInstancePushButton->setText("Create Instance");
    QObject::connect(createInstancePushButton, &QPushButton::clicked, this, &VulkanInstanceConfigWidget::OnCreateInstanceButtonClicked);
    bottomHBoxLayout->addWidget(createInstancePushButton);
}

void VulkanInstanceConfigWidget::DefaultVulkanInstanceLyaerAndExtensionConfig()
{
    std::vector<const char*> defaultInstanceLayers= { "VK_LAYER_KHRONOS_validation" };
    for(uint32_t i = 0; i < _InstanceLayerCheckBoxs.size(); i++){
        Qt::CheckState checkState = Qt::CheckState::Unchecked;
        for (auto& defaultInstanceLayer : defaultInstanceLayers) {
            if(strcmp(_InstanceLayerProperties[i].layerName, defaultInstanceLayer) == 0){
                checkState = Qt::CheckState::Checked;
                break;
            }
        }
        _InstanceLayerCheckBoxs[i]->setCheckState(checkState);
    }

    std::vector<const char*> defaultInstanceExtensions = VulkanInstance::GetRequiredInstanceExtensions();
    defaultInstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    for(uint32_t i = 0; i < _InstanceExtensionCheckBoxs.size(); i++){
        Qt::CheckState checkState = Qt::CheckState::Unchecked;
        for (auto& defaultInstanceExtension : defaultInstanceExtensions) {
            if(strcmp(_InstanceExtensionProperties[i].extensionName, defaultInstanceExtension) == 0){
                checkState = Qt::CheckState::Checked;
                break;
            }
        }
        _InstanceExtensionCheckBoxs[i]->setCheckState(checkState);
    }
}

void VulkanInstanceConfigWidget::SaveVulkanInstanceLyaerAndExtensionConfig()
{
    // save config
    QSettings settings;
    uint32_t arrayIndex;
    settings.beginGroup("Vulkan");

    // save enabled instance layers
    arrayIndex = 0;
    settings.beginWriteArray("EnabledInstanceLayers");
    for(uint32_t i = 0; i < _InstanceLayerCheckBoxs.size(); i++){
        if(_InstanceLayerCheckBoxs[i]->isChecked()){
            settings.setArrayIndex(arrayIndex++);
            settings.setValue("EnabledInstanceLayerName", _InstanceLayerProperties[i].layerName);
        }
    }
    settings.endArray();
    // save enabled instance extensions
    arrayIndex = 0;
    settings.beginWriteArray("EnabledInstanceExtensions");
    for(uint32_t i = 0; i < _InstanceExtensionCheckBoxs.size(); i++){
        if(_InstanceExtensionCheckBoxs[i]->isChecked()){
            settings.setArrayIndex(arrayIndex++);
            settings.setValue("EnabledInstanceExtensionName", _InstanceExtensionProperties[i].extensionName);
        }
    }
    settings.endArray();

    settings.endGroup();
}

void VulkanInstanceConfigWidget::LoadVulkanInstanceLyaerAndExtensionConfig()
{
    //load config
    QSettings settings;
    int32_t arraySize;
    settings.beginGroup("Vulkan");
    // load previous enabled instance layers
    std::vector<QString> loadInstanceLayers;
    arraySize = settings.beginReadArray("EnabledInstanceLayers");
    for (int32_t i = 0; i < arraySize; i++) {
        settings.setArrayIndex(i);
        loadInstanceLayers.push_back(settings.value("EnabledInstanceLayerName").toString());
    }
    settings.endArray();
    for(uint32_t i = 0; i < _InstanceLayerCheckBoxs.size(); i++){
        Qt::CheckState checkState = Qt::CheckState::Unchecked;
        for (auto& loadInstanceLayer : loadInstanceLayers) {
            if(loadInstanceLayer.compare(_InstanceLayerProperties[i].layerName) == 0){
                checkState = Qt::CheckState::Checked;
                break;
            }
        }
        _InstanceLayerCheckBoxs[i]->setCheckState(checkState);
    }

    // load previous enabled instance extensions
    std::vector<QString> loadInstanceExtensions;
    arraySize = settings.beginReadArray("EnabledInstanceExtensions");
    for (int32_t i = 0; i < arraySize; i++) {
        settings.setArrayIndex(i);
        loadInstanceExtensions.push_back(settings.value("EnabledInstanceExtensionName").toString());
    }
    settings.endArray();
    for(uint32_t i = 0; i < _InstanceExtensionCheckBoxs.size(); i++){
        Qt::CheckState checkState = Qt::CheckState::Unchecked;
        for (auto& loadInstanceExtension : loadInstanceExtensions) {
            if(loadInstanceExtension.compare(_InstanceExtensionProperties[i].extensionName) == 0){
                checkState = Qt::CheckState::Checked;
                break;
            }
        }
        _InstanceExtensionCheckBoxs[i]->setCheckState(checkState);
    }

    settings.endGroup();
}

void VulkanInstanceConfigWidget::OnCreateInstanceButtonClicked()
{
    std::vector<const char*> enabledInstanceLayerNames;
    std::vector<const char*> enabledInstanceExtensionNames;
    for (uint32_t i = 0; i < _InstanceLayerCheckBoxs.size(); i++) {
        if(_InstanceLayerCheckBoxs[i]->isChecked())
        {
            enabledInstanceLayerNames.push_back(_InstanceLayerProperties[i].layerName);
        }
    }
    for (uint32_t i = 0; i < _InstanceExtensionCheckBoxs.size(); i++) {
        if(_InstanceExtensionCheckBoxs[i]->isChecked())
        {
            enabledInstanceExtensionNames.push_back(_InstanceExtensionProperties[i].extensionName);
        }
    }
    _pMainWindow->CreateVulkanInstance(enabledInstanceLayerNames, enabledInstanceExtensionNames);
    SaveVulkanInstanceLyaerAndExtensionConfig();
}

void VulkanInstanceConfigWidget::OnDefaultSettingsButtonClicked()
{
    DefaultVulkanInstanceLyaerAndExtensionConfig();
}
