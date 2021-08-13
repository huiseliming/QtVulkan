#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <VulkanGraphics.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void CreateVulkanLyaerAndExtensionSelector(std::vector<VkLayerProperties> layerProperties, std::vector<VkExtensionProperties> extensionProperties);

public slots:
    void SelectVulkanLyaerAndExtension();

public:
    VulkanGraphics graphics;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
