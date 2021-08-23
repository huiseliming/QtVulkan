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
            QObject::connect(radioButton, &QRadioButton::clicked, [this, topHBoxLayout, i] {
                auto physicalDeviceInfo = _PhysicalDeviceInfos[i];
                _extensionNames.clear();
                _SelectedIndex = i;
                _pExtensionListWidget->deleteLater();
                _pExtensionListWidget = new QListWidget(this);
                for (uint32_t j = 0; j < physicalDeviceInfo._SupportedExtensionProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox(_pExtensionListWidget);
                    checkBox->setText(physicalDeviceInfo._SupportedExtensionProperties[j].extensionName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    _pExtensionListWidget->addItem(listWidgetItem);
                    _pExtensionListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, checkBox, i, j]{
                        auto physicalDeviceInfo = _PhysicalDeviceInfos[i];
                        if (checkBox->isChecked()) {
                            _extensionNames.push_back(physicalDeviceInfo._SupportedExtensionProperties[j].extensionName);
                        }else{
                            _extensionNames.erase(std::remove(std::begin(_extensionNames), std::end(_extensionNames), _PhysicalDeviceInfos[i]._SupportedExtensionProperties[j].extensionName), _extensionNames.end());
                        }
                    });
                }
                topHBoxLayout->insertWidget(1,_pExtensionListWidget);
                QString infoText;
                infoText.reserve(4096);
                infoText += QString("DeviceType : %1\n").arg(VulkanTools::ToString(physicalDeviceInfo._PhysicalDeviceProperties.deviceType));
                infoText += QString("DeviceLocalMemorySize : %1 MB\n").arg(static_cast<int32_t>(physicalDeviceInfo.GetDeviceLocalMemorySize() / 1024UL / 1024UL));
                for (uint32_t j = 0; j < physicalDeviceInfo._QueueFamilyProperties.size(); j++) {
                    infoText += QString("QueueCount : %1\n").arg(static_cast<int32_t>(physicalDeviceInfo._QueueFamilyProperties[j].queueCount));
                    infoText += "QueueFlag  : ";
                    std::vector<const char*> flagStrings;
                    if(physicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                        flagStrings.push_back("Graphics");
                    }
                    if(physicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT){
                        flagStrings.push_back("Compute");
                    }
                    if(physicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_TRANSFER_BIT){
                        flagStrings.push_back("Transfer");
                    }
                    if(physicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
                        flagStrings.push_back("SparseBinding");
                    }
                    if(physicalDeviceInfo._QueueFamilyProperties[j].queueFlags & VK_QUEUE_PROTECTED_BIT){
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
        _pExtensionListWidget = new QListWidget(this);
        topHBoxLayout->addWidget(_pExtensionListWidget);
    }
    {

        _pTextBrowser = new QTextBrowser(this);
        topHBoxLayout->addWidget(_pTextBrowser);
    }
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
    std::vector<const char*> enabledLayerNames;
    std::vector<const char*> enabledExtensionNames;
    _pMainWindow->CreateVulkanDevice(_PhysicalDeviceInfos[_SelectedIndex], enabledLayerNames, enabledExtensionNames, VkPhysicalDeviceFeatures{});
}
