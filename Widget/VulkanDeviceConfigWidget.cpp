#include "VulkanDeviceConfigWidget.h"

#include <QLayout>
#include <QListWidget>
#include <QRadioButton>
#include <QPushButton>

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
            QObject::connect(radioButton, &QRadioButton::clicked, [&, i] {
                _SelectedIndex = i;
            });
        }
        topHBoxLayout->addWidget(listWidget);
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
