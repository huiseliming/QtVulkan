#include "mainwindow.h"
#include "VulkanConfigWidget.h"

#include <QApplication>
#include <QVulkanInstance>
#include <VulkanInstance.h>
int main(int argc, char *argv[])
{
    auto layers = VulkanInstance::EnumerateInstanceLayerProperties();
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Hslm");
    QCoreApplication::setOrganizationDomain("unknow");
    QCoreApplication::setApplicationName("QtVulkan");

    MainWindow mainWindow;
    VulkanConfigWidget* pVulkanConfigWidget = new VulkanConfigWidget(&mainWindow);
//    w.CreateVulkanLyaerAndExtensionSelector(layers, exts);
//    VulkanInstance instance;
//    instance.CreateInstance();
    mainWindow.setCentralWidget(pVulkanConfigWidget);
    mainWindow.show();
    return app.exec();
}
