#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>
#include "VulkanInstance.h"
#include "VulkanTools.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    try {
        QCoreApplication::setOrganizationName("Hslm");
        QCoreApplication::setOrganizationDomain("unknow");
        QCoreApplication::setApplicationName("QtVulkan");

        MainWindow mainWindow;

    //    w.CreateVulkanLyaerAndExtensionSelector(layers, exts);
    //    VulkanInstance instance;
    //    instance.CreateInstance();
        mainWindow.show();
        return app.exec();
    } catch (const VkException& e) {
        qDebug() << "[VkException] " << e.what();
    } catch (const std::exception e){
        qDebug() << "[VkException] " << e.what();
    } catch (...){
        qDebug() << "[UnknowException] ???" ;
    }
}
