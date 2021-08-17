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
    for (int32_t i = 0; i < _PhysicalDevices.size(); i++) {
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
        for (int32_t i = 0; i < _PhysicalDevices.size(); i++) {
            QRadioButton* radioButton = new QRadioButton(this);
            radioButton->setText(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceName);
            QListWidgetItem* listWidgetItem = new QListWidgetItem();
            listWidget->addItem(listWidgetItem);
            listWidget->setItemWidget(listWidgetItem, radioButton);
            QObject::connect(radioButton, &QRadioButton::clicked, [this, topHBoxLayout, i] {
                _extensionNames.clear();
                _SelectedIndex = i;
                _pExtensionListWidget->deleteLater();
                _pExtensionListWidget = new QListWidget(this);
                for (uint32_t j = 0; j < _PhysicalDeviceInfos[i]._SupportedExtensionProperties.size(); j++) {
                    QCheckBox* checkBox = new QCheckBox(_pExtensionListWidget);
                    checkBox->setText(_PhysicalDeviceInfos[i]._SupportedExtensionProperties[j].extensionName);
                    QListWidgetItem* listWidgetItem = new QListWidgetItem();
                    _pExtensionListWidget->addItem(listWidgetItem);
                    _pExtensionListWidget->setItemWidget(listWidgetItem, checkBox);
                    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, checkBox, i, j]{
                        if (checkBox->isChecked()) {
                            _extensionNames.push_back(_PhysicalDeviceInfos[i]._SupportedExtensionProperties[j].extensionName);
                        }else{
                            _extensionNames.erase(std::remove(std::begin(_extensionNames), std::end(_extensionNames), _PhysicalDeviceInfos[i]._SupportedExtensionProperties[j].extensionName), _extensionNames.end());
                        }
                    });
                }
                topHBoxLayout->insertWidget(1,_pExtensionListWidget);
                QString infoText;
                infoText += QString::asprintf("DeviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
                infoText += QString::asprintf("DeviceLocalMemorySize : %d MB\n", static_cast<int32_t>(_PhysicalDeviceInfos[i].GetDeviceLocalMemorySize() / 1024UL / 1024UL));
//                infoText += QString::asprintf("Supported Queue : %s\n", (_PhysicalDeviceInfos[i]._QueueFamilyProperties[0].queueFlags));
//                infoText += QString::asprintf("deviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
//                infoText += QString::asprintf("deviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
//                infoText += QString::asprintf("deviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
//                infoText += QString::asprintf("deviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
//                infoText += QString::asprintf("deviceType : %s\n", ToString(_PhysicalDeviceInfos[i]._PhysicalDeviceProperties.deviceType));
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
