#include "VulkanDeviceConfigWidget.h"

#include <QLayout>
#include <QListWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <algorithm>
#include <iterator>
#include <QTextBrowser>
#include <vector>

VulkanDeviceConfigWidget::VulkanDeviceConfigWidget(QWidget *parent)
    : QWidget(parent)
    , _pMainWindow(reinterpret_cast<MainWindow*>(parent))
{
    _PhysicalDevices = VulkanDevice::GetPhysicalDevices(*_pMainWindow->_pGraphics->_Instance.get());
    for (uint32_t i = 0; i < _PhysicalDevices.size(); i++) {
        _PhysicalDeviceInfos.emplace_back(VulkanPhysicalDeviceInfo(_PhysicalDevices[i]));
    }
    CreateVulkanDeviceSelector();
}

void VulkanDeviceConfigWidget::CreateVulkanDeviceSelector()
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    QHBoxLayout* topHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(topHBoxLayout);
    {
        QListWidget* listWidget = new QListWidget(this);
        for (uint32_t i = 0; i < _PhysicalDevices.size(); i++) {
            QRadioButton* radioButton = new QRadioButton(this);
            radioButton->setText(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceName);
            QListWidgetItem* listWidgetItem = new QListWidgetItem();
            listWidget->addItem(listWidgetItem);
            listWidget->setItemWidget(listWidgetItem, radioButton);
            QObject::connect(radioButton, &QRadioButton::clicked, [this, i] {
                auto selectedPhysicalDeviceInfo = _PhysicalDeviceInfos[i];
                _SelectedDeviceIndex = i;

                // LayerName
                _LayerNames.clear();
                _pLayerNameListWidget->clear();
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo._SupportedLayerProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox();
                    checkBox->setText(selectedPhysicalDeviceInfo._SupportedLayerProperties[j].layerName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    _pLayerNameListWidget->addItem(listWidgetItem);
                    _pLayerNameListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, i, j] (int state) {
                        auto selectedPhysicalDeviceInfo = _PhysicalDeviceInfos[i];
                        if (state == Qt::CheckState::Checked) {
                            _LayerNames.push_back(selectedPhysicalDeviceInfo._SupportedLayerProperties[j].layerName);
                        }else{
                            _LayerNames.erase(std::remove(std::begin(_LayerNames), std::end(_LayerNames), selectedPhysicalDeviceInfo._SupportedLayerProperties[j].layerName), _LayerNames.end());
                        }
                    });
                }

                // ExtensionName
                _ExtensionNames.clear();
                _pExtensionNameListWidget->clear();
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo._SupportedExtensionProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox();
                    checkBox->setText(selectedPhysicalDeviceInfo._SupportedExtensionProperties[j].extensionName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    _pExtensionNameListWidget->addItem(listWidgetItem);
                    _pExtensionNameListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, i, j] (int state) {
                        auto selectedPhysicalDeviceInfo = _PhysicalDeviceInfos[i];
                        if (state == Qt::CheckState::Checked) {
                            _ExtensionNames.push_back(selectedPhysicalDeviceInfo._SupportedExtensionProperties[j].extensionName);
                        }else{
                            _ExtensionNames.erase(std::remove(std::begin(_ExtensionNames), std::end(_ExtensionNames), selectedPhysicalDeviceInfo._SupportedExtensionProperties[j].extensionName), _ExtensionNames.end());
                        }
                    });
                }

                QString infoText;
                infoText.reserve(4096);
                infoText += QString("DeviceType : %1\n").arg(VulkanTools::ToString(selectedPhysicalDeviceInfo._PhysicalDeviceProperties.deviceType));
                infoText += QString("DeviceLocalMemorySize : %1 MB\n").arg(static_cast<int32_t>(selectedPhysicalDeviceInfo.GetDeviceLocalMemorySize() / 1024UL / 1024UL));
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo._QueueFamilyProperties.size(); j++) {
                    infoText += QString("QueueCount : %1\n").arg(static_cast<int32_t>(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueCount));
                    infoText += "QueueFlag  : ";
                    std::vector<const char*> flagStrings;
                    if(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                        flagStrings.push_back("Graphics");
                    }
                    if(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT){
                        flagStrings.push_back("Compute");
                    }
                    if(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_TRANSFER_BIT){
                        flagStrings.push_back("Transfer");
                    }
                    if(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
                        flagStrings.push_back("SparseBinding");
                    }
                    if(selectedPhysicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_PROTECTED_BIT){
                        flagStrings.push_back("Protected");
                    }
                    for (uint32_t k = 0; k < flagStrings.size() - 1; k++) {
                        infoText += flagStrings[k];
                        infoText += " | ";
                    }
                    infoText += flagStrings[flagStrings.size() - 1];
                    infoText += "\n";
                }
                _pTextBrowser->setText(infoText);
            });
        }
        topHBoxLayout->addWidget(listWidget);
    }
    {
        _pLayerNameListWidget = new QListWidget(this);
        topHBoxLayout->addWidget(_pLayerNameListWidget);
        _pExtensionNameListWidget = new QListWidget(this);
        topHBoxLayout->addWidget(_pExtensionNameListWidget);
    }

    QHBoxLayout* centerHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(centerHBoxLayout);

    _pTextBrowser = new QTextBrowser(this);
    centerHBoxLayout->addWidget(_pTextBrowser);

    QHBoxLayout* bottomHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(bottomHBoxLayout);

    // create create device button
    QPushButton* createInstancePushButton = new QPushButton(this);
    createInstancePushButton->setText("Create Device");
    QObject::connect(createInstancePushButton, &QPushButton::clicked, this, &VulkanDeviceConfigWidget::OnCreateDeviceButtonClicked);
    bottomHBoxLayout->addWidget(createInstancePushButton);
}

void VulkanDeviceConfigWidget::OnCreateDeviceButtonClicked()
{
    _pMainWindow->CreateVulkanDevice(_PhysicalDeviceInfos[_SelectedDeviceIndex], _LayerNames, _ExtensionNames, VkPhysicalDeviceFeatures{});
}
