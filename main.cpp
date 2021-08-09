#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>
#include <VulkanInstance.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    auto layers = VulkanInstance::EnumerateInstanceLayerProperties();
    for(auto layer : layers)
    {
        std::printf("layerName            : %s\n", layer.layerName);
        std::printf("description          : %s\n", layer.description);
        std::printf("implementationVersion: %d\n", layer.implementationVersion);
        std::printf("specVersion          : %d\n", layer.specVersion);
    }
    auto exts = VulkanInstance::EnumerateInstanceExtensionProperties();
    for(auto ext : exts)
    {
        std::printf("extensionName: %s\n", ext.extensionName);
        std::printf("specVersion  : %d\n", ext.specVersion);
    }
    VulkanInstance instance;
    instance.CreateInstance();
    w.show();
    return a.exec();
}
