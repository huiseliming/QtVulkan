#include "VulkanDeviceConfigWidget.h"

#include <QLayout>
#include <QListWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <algorithm>
#include <iterator>
#include <QTextBrowser>

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
                std::optional<uint32_t> graphicsQueueIndex = physicalDeviceInfo.GetGraphicsQueueIndex();
                std::optional<uint32_t> computeQueueIndex = physicalDeviceInfo.GetComputeQueueIndex();
                std::optional<uint32_t> transferQueueIndex = physicalDeviceInfo.GetTransferQueueIndex();
                infoText += QString::asprintf("DeviceType            : %s\n", ToString(physicalDeviceInfo._PhysicalDeviceProperties.deviceType));
                infoText += QString::asprintf("DeviceLocalMemorySize : %d MB\n", static_cast<int32_t>(physicalDeviceInfo.GetDeviceLocalMemorySize() / 1024UL / 1024UL));
                infoText += QString::asprintf("GraphicsQueueCount    : %d\n", graphicsQueueIndex.has_value() ? physicalDeviceInfo._QueueFamilyProperties[graphicsQueueIndex.value()].queueCount : 0);
                infoText += QString::asprintf("ComputeQueueIndex     : %d\n", computeQueueIndex.has_value() ? physicalDeviceInfo._QueueFamilyProperties[computeQueueIndex.value()].queueCount : 0);
                infoText += QString::asprintf("TransferQueueIndex    : %d\n", transferQueueIndex.has_value() ? physicalDeviceInfo._QueueFamilyProperties[transferQueueIndex.value()].queueCount : 0);
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

}
