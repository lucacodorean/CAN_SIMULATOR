#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <vector>
#include <windows.h>
#include "linewidget.h"
#include "can_node.h"
#include "SimulationController.cpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<LineWidget*> getBus() { return this->bus; }
    std::vector<CAN_NODE*> getNodes() { return this->nodes; }
    Ui::MainWindow *ui;

    void insertNode(CAN_NODE* value) {
        if (value == nullptr) {
            qDebug("nullptr encountered.");
            return;
        }

        this->nodes.push_back(value);
    }

    void insertSegment(LineWidget* value) {
        if (value == nullptr) {
            qDebug("nullptr encountered.");
            return;
        }

        this->bus.push_back(value);
    }

    void resetNodes() { this->nodes.clear(); }
    void resetBus()   { this->bus.clear(); }

private slots:
    void on_firstExample_triggered();

    void on_cbNodes_currentIndexChanged(int index);

    void on_actionReset_triggered();

    void on_actionServer_Node_triggered();

    void on_actionArithmetic_Logic_Client_Node_triggered();

    void on_actionRGB_Client_Node_triggered();

    void on_actionSimulate_triggered();

    void on_secondExample_triggered();

    void on_thirdExample_triggered();

    void on_clientServerExample_triggered();

    void onNextStep();

    void on_fifthExample_triggered();

    void on_sixthExample_triggered();

private:

    std::vector<CAN_NODE*> nodes;
    std::vector<LineWidget*> bus;

    SimulationController* controller;

    HANDLE pipeW, pipeR;
};
#endif // MAINWINDOW_H
