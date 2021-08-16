#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <VulkanGraphics.h>
class VulkanInstanceConfigWidget;
class VulkanDeviceConfigWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void CreateVulkanInstance(std::vector<const char*>& enabledInstanceLayers, std::vector<const char*>& enabledInstanceExtensions);

public:
    std::unique_ptr<VulkanGraphics> _pGraphics;

private:
    Ui::MainWindow *ui;

    VulkanInstanceConfigWidget* _pVulkanInstanceConfigWidget{nullptr};
    VulkanDeviceConfigWidget* _pVulkanDeviceConfigWidget{nullptr};
};

extern MainWindow* GMainWindow;

#endif // MAINWINDOW_H
