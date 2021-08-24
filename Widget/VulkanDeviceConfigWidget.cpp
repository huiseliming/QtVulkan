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
    , pMainWindow(reinterpret_cast<MainWindow*>(parent))
{
    PhysicalDevices = VulkanDevice::GetPhysicalDevices(*pMainWindow->pGraphics->pInstance.get());
    for (uint32_t i = 0; i < PhysicalDevices.size(); i++) {
        PhysicalDeviceInfos.emplace_back(VulkanPhysicalDeviceInfo(PhysicalDevices[i]));
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
        for (uint32_t i = 0; i < PhysicalDevices.size(); i++) {
            QRadioButton* radioButton = new QRadioButton(this);
            radioButton->setText(PhysicalDeviceInfos[i].PhysicalDeviceProperties.deviceName);
            QListWidgetItem* listWidgetItem = new QListWidgetItem();
            listWidget->addItem(listWidgetItem);
            listWidget->setItemWidget(listWidgetItem, radioButton);
            QObject::connect(radioButton, &QRadioButton::clicked, [this, i] {
                auto selectedPhysicalDeviceInfo = PhysicalDeviceInfos[i];
                SelectedDeviceIndex = i;

                // LayerName
                LayerNames.clear();
                pLayerNameListWidget->clear();
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo.SupportedLayerProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox();
                    checkBox->setText(selectedPhysicalDeviceInfo.SupportedLayerProperties[j].layerName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    pLayerNameListWidget->addItem(listWidgetItem);
                    pLayerNameListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, i, j] (int state) {
                        auto selectedPhysicalDeviceInfo = PhysicalDeviceInfos[i];
                        if (state == Qt::CheckState::Checked) {
                            LayerNames.push_back(selectedPhysicalDeviceInfo.SupportedLayerProperties[j].layerName);
                        }else{
                            LayerNames.erase(std::remove(std::begin(LayerNames), std::end(LayerNames), selectedPhysicalDeviceInfo.SupportedLayerProperties[j].layerName), LayerNames.end());
                        }
                    });
                }

                // ExtensionName
                ExtensionNames.clear();
                pExtensionNameListWidget->clear();
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo.SupportedExtensionProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox();
                    checkBox->setText(selectedPhysicalDeviceInfo.SupportedExtensionProperties[j].extensionName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    pExtensionNameListWidget->addItem(listWidgetItem);
                    pExtensionNameListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, i, j] (int state) {
                        auto selectedPhysicalDeviceInfo = PhysicalDeviceInfos[i];
                        if (state == Qt::CheckState::Checked) {
                            ExtensionNames.push_back(selectedPhysicalDeviceInfo.SupportedExtensionProperties[j].extensionName);
                        }else{
                            ExtensionNames.erase(std::remove(std::begin(ExtensionNames), std::end(ExtensionNames), selectedPhysicalDeviceInfo.SupportedExtensionProperties[j].extensionName), ExtensionNames.end());
                        }
                    });
                }

                QString infoText;
                infoText.reserve(4096);
                infoText += QString("DeviceType : %1\n").arg(VulkanTools::ToString(selectedPhysicalDeviceInfo.PhysicalDeviceProperties.deviceType));
                infoText += QString("DeviceLocalMemorySize : %1 MB\n").arg(static_cast<int32_t>(selectedPhysicalDeviceInfo.GetDeviceLocalMemorySize() / 1024UL / 1024UL));
                for (uint32_t j = 0; j < selectedPhysicalDeviceInfo.QueueFamilyProperties.size(); j++) {
                    infoText += QString("QueueCount : %1\n").arg(static_cast<int32_t>(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueCount));
                    infoText += "QueueFlag  : ";
                    std::vector<const char*> flagStrings;
                    if(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                        flagStrings.push_back("Graphics");
                    }
                    if(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT){
                        flagStrings.push_back("Compute");
                    }
                    if(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueFlags & VK_QUEUE_TRANSFER_BIT){
                        flagStrings.push_back("Transfer");
                    }
                    if(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
                        flagStrings.push_back("SparseBinding");
                    }
                    if(selectedPhysicalDeviceInfo.QueueFamilyProperties[j].queueFlags & VK_QUEUE_PROTECTED_BIT){
                        flagStrings.push_back("Protected");
                    }
                    for (uint32_t k = 0; k < flagStrings.size() - 1; k++) {
                        infoText += flagStrings[k];
                        infoText += " | ";
                    }
                    infoText += flagStrings[flagStrings.size() - 1];
                    infoText += "\n";
                }
                pTextBrowser->setText(infoText);
            });
        }
        topHBoxLayout->addWidget(listWidget);
    }
    {
        pLayerNameListWidget = new QListWidget(this);
        topHBoxLayout->addWidget(pLayerNameListWidget);
        pExtensionNameListWidget = new QListWidget(this);
        topHBoxLayout->addWidget(pExtensionNameListWidget);
    }

    QHBoxLayout* centerHBoxLayout = new QHBoxLayout();
    vBoxLayout->addLayout(centerHBoxLayout);

    pTextBrowser = new QTextBrowser(this);
    centerHBoxLayout->addWidget(pTextBrowser);

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
    pMainWindow->CreateVulkanDevice(PhysicalDeviceInfos[SelectedDeviceIndex], LayerNames, ExtensionNames, VkPhysicalDeviceFeatures{});
}
