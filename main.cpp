#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>
#include "VulkanInstance.h"
#include "VulkanTools.h"
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    try {
        QCoreApplication::setOrganizationName("Hslm");
        QCoreApplication::setOrganizationDomain("unknow");
        QCoreApplication::setApplicationName("QtVulkan");
        const QFont defaultFixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        app.setFont(defaultFixedFont);

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
