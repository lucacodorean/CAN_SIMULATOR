#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "alclientnode.h"
#include "rgbclientnode.h"
#include "clientnode.h"
#include "servernode.h"
#include "linewidget.h"
#include "INode.h"
#include <thread>
#include <chrono>

#ifndef __WAIT_INSTRUCTION__

#define WAIT_SIM(BUTTON) {  }
    #define __WAIT_INSTRUCTION__
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    ui->actionArithmetic_Logic_Client_Node->setEnabled(false);
    ui->actionRGB_Client_Node->setEnabled(false);

    controller = new SimulationController();
    connect(ui->btnNextStep, &QPushButton::clicked, this, &MainWindow::onNextStep);
}

int simulation[8] = {0};

MainWindow::~MainWindow() { delete ui; }
void MainWindow::onNextStep() { controller->executeNextStep(ui); }

void controlActions(Ui::MainWindow* ui) {
    ui->actionServer_Node->setEnabled(false);
    ui->actionSimulate->setEnabled(true);
    ui->firstExample->setEnabled(false);
    ui->secondExample->setEnabled(false);
    ui->thirdExample->setEnabled(false);
    ui->clientServerExample->setEnabled(false);
    ui->fifthExample->setEnabled(false);
    ui->sixthExample->setEnabled(false);
}

void MainWindow::on_cbNodes_currentIndexChanged(int index) {

    if(this->getNodes().size() == 0 || ui->cbNodes->count() == 0) return;

    INode* item = dynamic_cast<INode*>(this->getNodes().at(index));
    ui->txtCurrentData->setText(item->toString());
}

void MainWindow::on_actionReset_triggered() {
    for(auto busSegment  : this->getBus())   delete  busSegment;
    for(auto currentNode : this->getNodes()) delete currentNode;

    std::remove("CAN_BUS.pipe");

    this->resetBus();
    this->resetNodes();

    ui->txtCurrentData->setText("");
    ui->cbNodes->clear();
    ui->txtLog->clear();

    ui->actionServer_Node->setEnabled(true);
    ui->actionArithmetic_Logic_Client_Node->setEnabled(false);
    ui->actionRGB_Client_Node->setEnabled(false);
    ui->firstExample->setEnabled(true);
    ui->secondExample->setEnabled(true);
    ui->thirdExample->setEnabled(true);
    ui->clientServerExample->setEnabled(true);
    ui->fifthExample->setEnabled(true);
    ui->sixthExample->setEnabled(true);

    for(int i = 0; i<8; i++) simulation[i] = 0;
}

void MainWindow::on_actionServer_Node_triggered() {
    if(this->getNodes().size() == 1) return;

    ui->actionArithmetic_Logic_Client_Node->setEnabled(true);
    ui->actionRGB_Client_Node->setEnabled(true);

    ServerNode* temp = new ServerNode(0x1, "CAN_SENSOR_1", ":/images/imagine_server.png", this);

    this->insertNode(temp);

    ui->actionServer_Node->setEnabled(false);
    ui->firstExample->setEnabled(false);

    temp->show();

    ui->txtLog->append("[NEW]: CAN_SENSOR_1 has been conneceted to the bus.");
}

void MainWindow::on_actionArithmetic_Logic_Client_Node_triggered() {
    DWORD identifier = this->getNodes().size();

    QString description = "CAN_NODE_";
    description.append(std::to_string(identifier));

    ClientNode* temp = new ALClientNode(identifier,  description, ":/images/imagine_calculator.png", this);
    this->insertNode(temp);
    ui->cbNodes->addItem(description);

    LineWidget* segmentBus = new LineWidget(temp, this->getNodes().at(0), this);
    segmentBus->setGeometry((*this).geometry());
    segmentBus->show();

    this->insertSegment(segmentBus);
    temp->show();

    ui->txtLog->append("[NEW]:" + description + " has been conneceted to the bus.");
}

void MainWindow::on_actionRGB_Client_Node_triggered() {
    DWORD identifier = this->getNodes().size();

    QString description = "CAN_NODE_";
    description.append(std::to_string(identifier));

    ClientNode* temp = new RGBClientNode(identifier,  description, "", this);
    this->insertNode(temp);
    ui->cbNodes->addItem(description);

    LineWidget* segmentBus = new LineWidget(temp, this->getNodes().at(0), this);
    segmentBus->setGeometry((*this).geometry());
    segmentBus->show();

    this->insertSegment(segmentBus);
    temp->show();

    ui->txtLog->append("[NEW]:" + description + " has been conneceted to the bus.");
}

void simulateFirstExample(Ui::MainWindow* ui, INode* nodeSv, INode* nodeAL, INode* nodeRGB) {
    ui->txtLog->append("[PRESET 1]: Preset loaded.");
    ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 has been connected to the bus.");
    ui->txtLog->append("[PRESET 1]: CAN_SENSOR_2 has been connected to the bus.");
    ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 has been connected to the bus.");

    if(nodeSv->handleFrame(
        new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x04941211, 0x11, 0x3),
        nodeAL
    )) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->handleFrame(
        new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x00941211, 0x11, 0x4),
        nodeAL
    )) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");

    nodeAL->handleFrame(NULL, nodeRGB);

    if(nodeAL->writeInBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 attempted to send a frame in bus.");

    if(nodeRGB->readFromBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_2 attempted to read a frame from bus.");

    nodeRGB->handleFrame(NULL, nodeAL);

    if(nodeRGB->writeInBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");
}

class ScenarioPreset1 : public Ui::Scenario {
public:
    ScenarioPreset1(QTextEdit* log, INode* nodeSv, INode* nodeAL, INode* nodeRGB) {
        addStep([log]() {
            log->append("[PRESET 1]: Preset loaded.");
            log->append("[PRESET 1]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 1]: CAN_SENSOR_2 has been connected to the bus.");
            log->append("[PRESET 1]: CAN_SENSOR_3 has been connected to the bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 1]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv, nodeAL]() {
            if(nodeSv->handleFrame(
                new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3),
                nodeAL
                )) log->append("[PRESET 1]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 1]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 1]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 1]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL, nodeRGB]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 1]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL->handleFrame(NULL, nodeRGB);
        });

        addStep([log, nodeAL]() {
            if(nodeAL->writeInBus()) log->append("[PRESET 1]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB, nodeAL]() {
            if(nodeRGB->readFromBus()) log->append("[PRESET 1]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_2 attempted to read a frame from the bus.");
            nodeRGB->handleFrame(NULL, nodeAL);
        });

        addStep([log, nodeRGB]() {
            if(nodeRGB->writeInBus()) log->append("[PRESET 1]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 1]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");
        });
    }
};

void MainWindow::on_firstExample_triggered() {
    ClientNode* nodeRGB = new RGBClientNode(0x1,  "CAN_SENSOR_2", nullptr, this);
    ClientNode* nodeAL  = new ALClientNode(0x2,   "CAN_SENSOR_3", ":/images/imagine_calculator.png", this);
    ServerNode* nodeSv  = new   ServerNode(0x3,   "CAN_SENSOR_1", ":/images/imagine_server.png", this);

    this->insertNode(nodeSv);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeAL);   ui->cbNodes->addItem("CAN_SENSOR_3");
    this->insertNode(nodeRGB);  ui->cbNodes->addItem("CAN_SENSOR_2");

    nodeSv->move(400,  150);     nodeSv->show();
    nodeAL->move(100,  150);    nodeAL->show();
    nodeRGB->move(700, 150);    nodeRGB->show();

    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeSv, nodeAL,  this));
    physicalBus.append(new LineWidget(nodeSv, nodeRGB, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[0] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset1>(ui->txtLog, nodeSv, nodeAL, nodeRGB));
}

void simulateSecondExample(Ui::MainWindow* ui, INode* nodeSv, INode* nodeAL, INode* nodeRGB) {
    ui->txtLog->append("[PRESET 2]: Preset loaded.");
    ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 has been connected to the bus.");
    ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 has been connected to the bus.");
    ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 has been connected to the bus.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSv->handleFrame(
            new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x04941211, 0x11, 0x3),
            nodeAL
            )) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->handleFrame(
            new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x00941211, 0x11, 0x4),
            nodeAL
            )) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");

    nodeAL->handleFrame(NULL, nodeRGB);

    if(nodeAL->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 attempted to send a frame in bus.");


    if(nodeRGB->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 attempted to read a frame from bus.");

    nodeRGB->handleFrame(NULL, nodeAL);

    if(nodeRGB->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");


    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 1]: CAN_SENSOR_3 attempted to read a frame from the bus.");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    if(nodeAL->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 attempted to send a frame in bus.");

    if(nodeRGB->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 attempted to read a frame from bus.");
    nodeRGB->handleFrame(NULL, nodeAL);

    if(nodeRGB->writeInBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");
}

class ScenarioPreset2 : public Ui::Scenario {
public:
    ScenarioPreset2(QTextEdit* log, INode* nodeSv, INode* nodeAL, INode* nodeRGB) {
        addStep([log]() {
            log->append("[PRESET 2]: Preset loaded.");
            log->append("[PRESET 2]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 2]: CAN_SENSOR_2 has been connected to the bus.");
            log->append("[PRESET 2]: CAN_SENSOR_3 has been connected to the bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv, nodeAL]() {
            if(nodeSv->handleFrame(
                    new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x04941211, 0x11, 0x3),
                    nodeAL
                    )) log->append("[PRESET 2]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 2]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv, nodeAL]() {
            if(nodeSv->handleFrame(
                    new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x00941211, 0x11, 0x4),
                    nodeAL
                    )) log->append("[PRESET 2]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 2]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL, nodeRGB]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL->handleFrame(NULL, nodeRGB);
        });

        addStep([log, nodeAL]() {
            if(nodeAL->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB, nodeAL]() {
            if(nodeRGB->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_2 attempted to read a frame from bus.");
            nodeRGB->handleFrame(NULL, nodeAL);
        });

        addStep([log, nodeRGB]() {
            if(nodeRGB->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL, nodeRGB]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL->handleFrame(NULL, nodeRGB);
        });

        addStep([log, nodeAL]() {
            if(nodeAL->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB, nodeAL]() {
            if(nodeRGB->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_2 attempted to read a frame from bus.");
            nodeRGB->handleFrame(NULL, nodeAL);
        });

        addStep([log, nodeRGB]() {
            if(nodeRGB->writeInBus()) log->append("[PRESET 2]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 2]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 2]: CAN_SENSOR_3 attempted to read a frame from the bus.");
        });
    }
};

void MainWindow::on_secondExample_triggered()
{
    ClientNode* nodeRGB = new RGBClientNode(0x1,  "CAN_SENSOR_2", nullptr, this);
    ClientNode* nodeAL  = new ALClientNode(0x2,   "CAN_SENSOR_3", ":/images/imagine_calculator.png", this);
    ServerNode* nodeSv  = new   ServerNode(0x3,   "CAN_SENSOR_1", ":/images/imagine_server.png", this);

    this->insertNode(nodeSv);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeAL);   ui->cbNodes->addItem("CAN_SENSOR_3");
    this->insertNode(nodeRGB);  ui->cbNodes->addItem("CAN_SENSOR_2");

    nodeSv->move(600,  150);     nodeSv->show();
    nodeAL->move(300,  150);    nodeAL->show();
    nodeRGB->move(900, 150);    nodeRGB->show();

    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeRGB, nodeSv, this));
    physicalBus.append(new LineWidget(nodeSv, nodeAL, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[1] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset2>(ui->txtLog, nodeSv, nodeAL, nodeRGB));
}

class ScenarioPreset3 : public Ui::Scenario {
public:
    ScenarioPreset3(QTextEdit* log, INode* nodeSv, INode* nodeAL, INode* nodeRGB, INode* nodeRGB2) {
        addStep([log]() {
            log->append("[PRESET 3]: Preset loaded.");
            log->append("[PRESET 3]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 3]: CAN_SENSOR_2 has been connected to the bus.");
            log->append("[PRESET 3]: CAN_SENSOR_3 has been connected to the bus.");
            log->append("[PRESET 3]: CAN_SENSOR_4 has been connected to the bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv, nodeAL]() {
            if(nodeSv->handleFrame(
                new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x05941211, 0x11, 0x3),
                nodeAL)) log->append("[PRESET 3]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 3]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL, nodeRGB]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL->handleFrame(NULL, nodeRGB);
        });

        addStep([log, nodeAL]() {
            if(nodeAL->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB, nodeAL]() {
            if(nodeRGB->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_2 attempted to read a frame from bus.");
            nodeRGB->handleFrame(NULL, nodeAL);
        });

        addStep([log, nodeRGB]() {
            if(nodeRGB->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });


        addStep([log, nodeAL]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
        });

        addStep([log, nodeSv, nodeAL]() {
            if(nodeSv->handleFrame(
                    new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x02B6B6B6, 0x11, 0x3),
                    nodeAL
                    )) log->append("[PRESET 3]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 3]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL, nodeRGB2]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL->handleFrame(NULL, nodeRGB2);
        });

        addStep([log, nodeAL]() {
            if(nodeAL->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB2, nodeAL]() {
            if(nodeRGB2->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_2 attempted to read a frame from bus.");
            nodeRGB2->handleFrame(NULL, nodeAL);
        });

        addStep([log, nodeRGB2]() {
            if(nodeRGB2->writeInBus()) log->append("[PRESET 3]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL]() {
            if(nodeAL->readFromBus()) log->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
        });
    }
};

void MainWindow::on_thirdExample_triggered()
{
    ClientNode* nodeRGB  = new RGBClientNode(0x1,  "CAN_SENSOR_2", nullptr, this);
    ClientNode* nodeAL   = new ALClientNode(0x2,   "CAN_SENSOR_3", ":/images/imagine_calculator.png", this);
    ServerNode* nodeSv   = new   ServerNode(0x3,   "CAN_SENSOR_1", ":/images/imagine_server.png", this);
    ClientNode* nodeRGB2 = new RGBClientNode(0x4,  "CAN_SENSOR_4", nullptr, this);

    this->insertNode(nodeSv);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeAL);   ui->cbNodes->addItem("CAN_SENSOR_3");
    this->insertNode(nodeRGB);  ui->cbNodes->addItem("CAN_SENSOR_2");
    this->insertNode(nodeRGB2); ui->cbNodes->addItem("CAN_SENSOR_4");

    nodeSv->move(20,   20);         nodeSv->show();
    nodeAL->move(300,  20);         nodeAL->show();
    nodeRGB->move(600,  20);        nodeRGB->show();
    nodeRGB2->move(300,  280);      nodeRGB2->show();

    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeAL, nodeSv, this));
    physicalBus.append(new LineWidget(nodeRGB, nodeSv, this));
    physicalBus.append(new LineWidget(nodeRGB2, nodeRGB, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[2] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset3>(ui->txtLog, nodeSv, nodeAL, nodeRGB, nodeRGB2));
}

void simulateThirdExample(Ui::MainWindow* ui, INode* nodeSv, INode* nodeAL, INode* nodeRGB, INode* nodeRGB2) {
    ui->txtLog->append("[PRESET 3]: Preset loaded.");
    ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 has been connected to the bus.");
    ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 has been connected to the bus.");
    ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 has been connected to the bus.");
    ui->txtLog->append("[PRESET 3]: CAN_SENSOR_4 has been connected to the bus.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSv->handleFrame(
            new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x05941211, 0x11, 0x3),
            nodeAL
            )) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
    nodeAL->handleFrame(NULL, nodeRGB);

    if(nodeAL->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to send a frame in bus.");

    if(nodeRGB->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 attempted to read a frame from bus.");
    nodeRGB->handleFrame(NULL, nodeAL);

    if(nodeRGB->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");


    if(nodeSv->handleFrame(
            new Frame(nodeSv->getIndentifier(), nodeAL->getIndentifier(), TRUE, 0x02B6B6B6, 0x11, 0x3),
            nodeAL
            )) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");
    nodeAL->handleFrame(NULL, nodeRGB2);

    if(nodeAL->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to send a frame in bus.");

    if(nodeRGB2->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 attempted to read a frame from bus.");
    nodeRGB2->handleFrame(NULL, nodeAL);

    if(nodeRGB2->writeInBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL->readFromBus()) ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 3]: CAN_SENSOR_3 attempted to read a frame from the bus.");

}

class ScenarioPreset4 : public Ui::Scenario {
public:
    ScenarioPreset4(QTextEdit* log, INode* nodeSv, INode* nodeRGB) {
        addStep([log]() {
            log->append("[PRESET 4]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 4]: CAN_SENSOR_2 has been connected to the bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 4]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv, nodeRGB]() {
            if(nodeSv->handleFrame(new Frame(nodeSv->getIndentifier(), nodeRGB->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3), nodeRGB))
                log->append("[PRESET 4]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 4]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 4]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv, nodeRGB]() {
            if(nodeRGB->readFromBus()) log->append("[PRESET 4]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_2 attempted to read a frame from the bus.");
            nodeRGB->handleFrame(NULL, nodeSv);
        });

        addStep([log, nodeRGB]() {
            if(nodeRGB->writeInBus()) log->append("[PRESET 4]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->writeInBus()) log->append("[PRESET 4]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSv]() {
            if(nodeSv->readFromBus()) log->append("[PRESET 4]: CAN_SENSOR_1 read a frame from bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_1 attempted to read a frame from the bus.");
        });
    }
};

void MainWindow::on_clientServerExample_triggered() {
    ClientNode* nodeRGB  = new RGBClientNode(0x1,  "CAN_SENSOR_2", nullptr, this);
    ServerNode* nodeSv   = new   ServerNode(0x3,   "CAN_SENSOR_1", ":/images/imagine_server.png", this);

    this->insertNode(nodeSv);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeRGB);  ui->cbNodes->addItem("CAN_SENSOR_2");

    nodeSv->move(400,  120);      nodeSv->show();
    nodeRGB->move(700,  120);     nodeRGB->show();

    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeSv, nodeRGB, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[3] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset4>(ui->txtLog, nodeSv, nodeRGB));
}

void simulateForthExample(Ui::MainWindow* ui, INode* nodeSv, INode* nodeRGB) {
    ui->txtLog->append("[PRESET 4]: Preset loaded.");
    ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 has been connected to the bus.");
    ui->txtLog->append("[PRESET 4]: CAN_SENSOR_2 has been connected to the bus.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSv->handleFrame(new Frame(nodeSv->getIndentifier(), nodeRGB->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3), nodeRGB))
        ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSv->writeInBus()) ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeRGB->readFromBus()) ui->txtLog->append("[PRESET 4]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_2 attempted to read a frame from the bus.");
    nodeRGB->handleFrame(NULL, nodeSv);

    if(nodeRGB->writeInBus()) ui->txtLog->append("[PRESET 4]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeSv->readFromBus()) ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 read a frame from bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_1 attempted to read a frame from the bus.");
}

class ScenarioPreset5 : public Ui::Scenario {
public:
    ////////                                1           2                   3           4               5               6                   7
    ScenarioPreset5(QTextEdit* log, INode* nodeSV1, INode* nodeAL1, INode* nodeAL2, INode* nodeSV2, INode* nodeSV3, INode* nodeRGB1, INode* nodeRGB2) {
        addStep([log]() {
            log->append("[PRESET 5]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_2 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_3 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_4 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_5 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_6 has been connected to the bus.");
            log->append("[PRESET 5]: CAN_SENSOR_7 has been connected to the bus.");
        });

        addStep([log, nodeSV1]() {
            if(nodeSV1->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSV1, nodeAL1]() {
            if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL1->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3), nodeAL1))
                log->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSV1, nodeAL1]() {
            if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL1->getIndentifier(), TRUE, 0x02B6B6B6, 0x11, 0x6), nodeAL1))
                log->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSV1, nodeAL2]() {
            if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL2->getIndentifier(), TRUE, 0x05941211, 0x11, 0x4), nodeAL2))
                log->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeSV1]() {
            if(nodeSV1->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeSV1]() {
            if(nodeSV1->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL1, nodeSV2]() {
            if(nodeAL1->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_2 read a frame from bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_2 attempted to read a frame from the bus.");
            nodeAL1->handleFrame(NULL, nodeSV2);
        });

        addStep([log, nodeAL2, nodeSV3]() {
            if(nodeAL2->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_3 read a frame from bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_3 attempted to read a frame from the bus.");
            nodeAL2->handleFrame(NULL, nodeSV3);
        });

        addStep([log, nodeAL1]() {
            if(nodeAL1->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_2 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_2 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL2]() {
            if(nodeAL2->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_3 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_3 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB1, nodeSV2]() {
            if(nodeSV2->readFromBus()) {
                log->append("[PRESET 5]: CAN_SENSOR_4 read a frame from bus.");
                Frame* temp = new Frame(nodeSV2->getIndentifier(), nodeRGB1->getIndentifier(), TRUE, nodeSV2->getCurrentFrame().getData(), 0x11, nodeSV2->getCurrentFrame().getPriority() + 1);
                nodeSV2->handleFrame(temp, nodeRGB1);
                qDebug()<<nodeSV2->getTopFrameString()<< " " << nodeSV2->getCurrentFrame().toPipeStructure();
            }
            else log->append("[PRESET 5]: CAN_SENSOR_4 attempted to read a frame from the bus.");
        });

        addStep([log, nodeSV2]() {
            if(nodeSV2->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_4 sent a frame in bus.");
            else log->append("[PRESET 4]: CAN_SENSOR_4 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB2, nodeSV3]() {
            if(nodeSV3->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_5 read a frame from bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_5 attempted to read a frame from the bus.");

            Frame* temp = new Frame(nodeSV3->getIndentifier(), nodeRGB2->getIndentifier(), TRUE, nodeSV3->getCurrentFrame().getData(), 0x11, nodeSV3->getCurrentFrame().getPriority() + 1);
            nodeSV3->handleFrame(temp, nodeRGB2);
            qDebug()<<nodeSV3->getTopFrameString();
        });

        addStep([log, nodeSV3]() {
            if(nodeSV3->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_5 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_5 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB1, nodeSV2]() {
            if(nodeRGB1->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_6 read a frame from bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_6 attempted to read a frame from the bus.");
            nodeRGB1->handleFrame(NULL, nodeSV2);
        });


        addStep([log, nodeRGB1]() {
            if(nodeRGB1->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_5 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_5 attempted to send a frame in bus.");
        });

        addStep([log, nodeSV2]() {
            if(nodeSV2->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_4 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_4 attempted to send a frame in bus.");
        });

        addStep([log, nodeRGB2, nodeSV3]() {
            if(nodeRGB2->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_7 read a frame from bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_7 attempted to read a frame from the bus.");
            nodeRGB2->handleFrame(NULL, nodeSV3);
        });

        addStep([log, nodeRGB2]() {
            if(nodeRGB2->writeInBus()) log->append("[PRESET 5]: CAN_SENSOR_7 sent a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_7 attempted to send a frame in bus.");
        });

        addStep([log, nodeSV3]() {
            if(nodeSV3->readFromBus()) log->append("[PRESET 5]: CAN_SENSOR_5 read a frame in bus.");
            else log->append("[PRESET 5]: CAN_SENSOR_5 attempted to read a frame in bus.");
        });
    }
};

void MainWindow::on_fifthExample_triggered() {

    ServerNode* nodeSV1    = new     ServerNode(0x1,        "CAN_SENSOR_1",    ":/images/imagine_server.png",           this);
    ClientNode* nodeAL1    = new   ALClientNode(0x2,        "CAN_SENSOR_2",    ":/images/imagine_calculator.png",       this);
    ClientNode* nodeAL2    = new   ALClientNode(0x3,        "CAN_SENSOR_3",    ":/images/imagine_calculator.png",       this);
    ServerNode* nodeSV2    = new     ServerNode(0x4,        "CAN_SENSOR_4",    ":/images/imagine_server.png",           this);
    ServerNode* nodeSV3    = new     ServerNode(0x5,        "CAN_SENSOR_5",    ":/images/imagine_server.png",           this);
    ClientNode* nodeRGB1   = new     RGBClientNode(0x6,     "CAN_SENSOR_6",    nullptr,                                 this);
    ClientNode* nodeRGB2   = new     RGBClientNode(0x7,     "CAN_SENSOR_7",    nullptr,                                 this);

    this->insertNode(nodeSV1);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeAL1);   ui->cbNodes->addItem("CAN_SENSOR_2");
    this->insertNode(nodeAL2);   ui->cbNodes->addItem("CAN_SENSOR_3");
    this->insertNode(nodeSV2);   ui->cbNodes->addItem("CAN_SENSOR_4");
    this->insertNode(nodeSV3);   ui->cbNodes->addItem("CAN_SENSOR_5");
    this->insertNode(nodeRGB1);  ui->cbNodes->addItem("CAN_SENSOR_6");
    this->insertNode(nodeRGB2);  ui->cbNodes->addItem("CAN_SENSOR_7");

    nodeSV1->move(20,   20);    nodeSV1->show();
    nodeAL1->move(300,  20);    nodeAL1->show();
    nodeAL2->move(600,  20);    nodeAL2->show();
    nodeSV2->move(665,  280);    nodeSV2->show();
    nodeSV3->move(1000,  280);    nodeSV3->show();
    nodeRGB1->move(550,  580);   nodeRGB1->show();
    nodeRGB2->move(1300, 280);   nodeRGB2->show();

    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeAL1, nodeSV1, this));
    physicalBus.append(new LineWidget(nodeAL2, nodeSV1, this));
    physicalBus.append(new LineWidget(nodeAL1, nodeSV2, this));
    physicalBus.append(new LineWidget(nodeAL2, nodeSV3, this));
    physicalBus.append(new LineWidget(nodeRGB1, nodeSV2, this));
    physicalBus.append(new LineWidget(nodeRGB2, nodeSV3, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[4] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset5>(ui->txtLog, nodeSV1, nodeAL1, nodeAL2, nodeSV2, nodeSV3, nodeRGB1, nodeRGB2));
}

void simulateFifthExample(Ui::MainWindow* ui, INode* nodeSV1, INode* nodeAL1, INode* nodeAL2, INode* nodeSV2, INode* nodeSV3, INode* nodeRGB1, INode* nodeRGB2) {
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_2 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_3 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_6 has been connected to the bus.");
    ui->txtLog->append("[PRESET 5]: CAN_SENSOR_7 has been connected to the bus.");


    if(nodeSV1->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL1->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3), nodeAL1))
        ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL1->getIndentifier(), TRUE, 0x02B6B6B6, 0x11, 0x6), nodeAL1))
        ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL2->getIndentifier(), TRUE, 0x05941211, 0x11, 0x4), nodeAL2))
        ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 has received a new frame.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 hasn't received a new frame.");

    if(nodeSV1->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeSV1->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_1 attempted to send a frame in bus.");

    if(nodeAL1->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_2 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_2 attempted to read a frame from the bus.");
    nodeAL1->handleFrame(NULL, nodeSV2);

    if(nodeAL2->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_3 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_3 attempted to read a frame from the bus.");
    nodeAL2->handleFrame(NULL, nodeSV3);

    if(nodeAL1->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_2 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_2 attempted to send a frame in bus.");

    if(nodeAL2->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_3 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_3 attempted to send a frame in bus.");

    if(nodeSV2->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 attempted to read a frame from the bus.");

    Frame* temp = new Frame(nodeSV2->getIndentifier(), nodeRGB1->getIndentifier(), TRUE, nodeSV2->getCurrentFrame().getData(), 0x11, nodeSV2->getCurrentFrame().getPriority() + 1);
    nodeSV2->handleFrame(temp, nodeRGB1);

    if(nodeSV2->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 4]: CAN_SENSOR_4 attempted to send a frame in bus.");

    if(nodeSV3->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 attempted to read a frame from the bus.");

    Frame* temp2 = new Frame(nodeSV3->getIndentifier(), nodeRGB2->getIndentifier(), TRUE, nodeSV3->getCurrentFrame().getData(), 0x11, nodeSV3->getCurrentFrame().getPriority() + 1);
    nodeSV3->handleFrame(temp2, nodeRGB2);

    if(nodeSV3->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 attempted to send a frame in bus.");

    if(nodeRGB1->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_6 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_6 attempted to read a frame from the bus.");
    nodeRGB1->handleFrame(NULL, nodeSV2);

    if(nodeRGB1->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_6 sent a frame in bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_6 attempted to send a frame in bus.");

    if(nodeSV2->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_4 attempted read a frame from bus.");

    if(nodeRGB2->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_7 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_7 attempted to read a frame from the bus.");
    nodeRGB2->handleFrame(NULL, nodeSV3);

    if(nodeRGB2->writeInBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_7 sent a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_7 attempted to send a frame from bus.");

    if(nodeSV3->readFromBus()) ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 read a frame from bus.");
    else ui->txtLog->append("[PRESET 5]: CAN_SENSOR_5 attempted to read a frame from bus.");
}

class ScenarioPreset6 : public Ui::Scenario {
public:
    ScenarioPreset6(QTextEdit* log, INode* nodeSV1, INode* nodeAL1, INode* nodeAL2) {
        addStep([log]() {
            log->append("[PRESET 6]: CAN_SENSOR_1 has been connected to the bus.");
            log->append("[PRESET 6]: CAN_SENSOR_2 has been connected to the bus.");
            log->append("[PRESET 6]: CAN_SENSOR_3 has been connected to the bus.");
        });

        addStep([log, nodeSV1, nodeAL2]() {
            if(nodeSV1->handleFrame(new Frame(nodeSV1->getIndentifier(), nodeAL2->getIndentifier(), TRUE, 0x00941211, 0x11, 0x3), nodeAL2))
                log->append("[PRESET 6]: CAN_SENSOR_1 has received a new frame.");
            else log->append("[PRESET 6]]: CAN_SENSOR_1 hasn't received a new frame.");
        });

        addStep([log, nodeAL1, nodeAL2]() {
            if(nodeAL1->readFromBus()) {
                log->append("[PRESET 6]: CAN_SENSOR_2 read a frame from bus.");
                nodeAL1->handleFrame(NULL, nodeAL2);
            }
            else log->append("[PRESET 6]: CAN_SENSOR_2 attempted to read a frame from the bus.");
        });

        addStep([log, nodeSV1]() {
            if(nodeSV1->writeInBus()) log->append("[PRESET 6]: CAN_SENSOR_1 sent a frame in bus.");
            else log->append("[PRESET 6]: CAN_SENSOR_1 attempted to send a frame in bus.");
        });

        addStep([log, nodeAL1, nodeAL2]() {
            if(nodeAL1->readFromBus()) {
                log->append("[PRESET 6]: CAN_SENSOR_2 read a frame from bus.");
                nodeAL1->handleFrame(NULL, nodeAL2);
            }
            else log->append("[PRESET 6]: CAN_SENSOR_2 attempted to read a frame from the bus.");
        });
    }
};

void MainWindow::on_sixthExample_triggered()
{
    ServerNode* nodeSV1    = new     ServerNode(0x1,        "CAN_SENSOR_1",    ":/images/imagine_server.png",           this);
    ClientNode* nodeAL1    = new   ALClientNode(0x2,        "CAN_SENSOR_2",    ":/images/imagine_calculator.png",       this);
    ClientNode* nodeAL2    = new   ALClientNode(0x3,        "CAN_SENSOR_3",    ":/images/imagine_calculator.png",       this);

    nodeSV1->move(20,   20);    nodeSV1->show();
    nodeAL1->move(300,  20);    nodeAL1->show();
    nodeAL2->move(600,  20);    nodeAL2->show();

    this->insertNode(nodeSV1);   ui->cbNodes->addItem("CAN_SENSOR_1");
    this->insertNode(nodeAL1);   ui->cbNodes->addItem("CAN_SENSOR_2");
    this->insertNode(nodeAL2);   ui->cbNodes->addItem("CAN_SENSOR_3");
    QList<LineWidget*> physicalBus;

    physicalBus.append(new LineWidget(nodeAL1, nodeSV1, this));
    physicalBus.append(new LineWidget(nodeAL2, nodeSV1, this));

    for(auto busSegment : physicalBus) {
        busSegment->setGeometry((*this).geometry());
        busSegment->show();
        this->insertSegment(busSegment);
    }

    controlActions(this->ui);
    simulation[5] = 1;

    controller->setScenario(std::make_unique<ScenarioPreset6>(ui->txtLog, nodeSV1, nodeAL1, nodeAL2));
}

void simulateSixthExample(Ui::MainWindow* ui, INode* nodeSV1, INode* nodeAL1, INode* nodeAL2) {

}

void MainWindow::on_actionSimulate_triggered() {
    if(simulation[0] == 1) {
        simulateFirstExample(this->ui,
             dynamic_cast<INode*>(this->getNodes().at(0)),
             dynamic_cast<INode*>(this->getNodes().at(1)),
             dynamic_cast<INode*>(this->getNodes().at(2))
         );

    }

    if(simulation[1] == 1) {
        simulateSecondExample(this->ui,
              dynamic_cast<INode*>(this->getNodes().at(0)),
              dynamic_cast<INode*>(this->getNodes().at(1)),
              dynamic_cast<INode*>(this->getNodes().at(2))
          );
    }

    if(simulation[2] == 1) {
        simulateThirdExample(this->ui,
             dynamic_cast<INode*>(this->getNodes().at(0)),
             dynamic_cast<INode*>(this->getNodes().at(1)),
             dynamic_cast<INode*>(this->getNodes().at(2)),
             dynamic_cast<INode*>(this->getNodes().at(3))
         );
    }

    if(simulation[3] == 1) {
        simulateForthExample(this->ui,
            dynamic_cast<INode*>(this->getNodes().at(0)),
            dynamic_cast<INode*>(this->getNodes().at(1))
        );
    }

    if(simulation[4] == 1) {
        simulateFifthExample(this->ui,
            dynamic_cast<INode*>(this->getNodes().at(0)),
            dynamic_cast<INode*>(this->getNodes().at(1)),
            dynamic_cast<INode*>(this->getNodes().at(2)),
            dynamic_cast<INode*>(this->getNodes().at(3)),
            dynamic_cast<INode*>(this->getNodes().at(4)),
            dynamic_cast<INode*>(this->getNodes().at(5)),
            dynamic_cast<INode*>(this->getNodes().at(6))
        );
    }

    if(simulation[5] == 1) {
        simulateSixthExample(
            this->ui,
            dynamic_cast<INode*>(this->getNodes().at(0)),
            dynamic_cast<INode*>(this->getNodes().at(1)),
            dynamic_cast<INode*>(this->getNodes().at(2))
        );
    }

    std::remove("CAN_BUS.pipe");
}

