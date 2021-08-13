#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCheckBox>
#include <CheckBoxDelegate.h>
#include <CheckBoxVectorModel.h>
#include <QHBoxLayout>
#include <QListView>
#include <QStringListModel>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::CreateVulkanLyaerAndExtensionSelector(std::vector<VkLayerProperties> layerProperties, std::vector<VkExtensionProperties> extensionProperties) 
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QListView* layerList = new QListView(this);
    CheckBoxVectorModel* model = new CheckBoxVectorModel(this);
    model->insertRows(0, layerProperties.size());
    for (int32_t i = 0; i < layerProperties.size(); i++) {
        QVariant variant;
        variant.setValue(CheckBoxModelData{.IsChecked = false, .DisplayString = layerProperties[i].layerName});
        model->setData(model->index(i, 0), variant);
    }
    CheckBoxDelegate* delegate = new CheckBoxDelegate(this);
    layerList->setModel(model);
    layerList->setItemDelegate(delegate);
    layout->addWidget(layerList);
    setLayout(layout);
}

void MainWindow::SelectVulkanLyaerAndExtension() 
{
}

