/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRGB_Client_Node;
    QAction *firstExample;
    QAction *actionSimulate;
    QAction *actionReset;
    QAction *actionArithmetic_Logic_Client_Node;
    QAction *actionServer_Node;
    QAction *secondExample;
    QAction *thirdExample;
    QAction *clientServerExample;
    QAction *actiontete;
    QAction *fifthExample;
    QAction *sixthExample;
    QWidget *centralwidget;
    QFrame *frame;
    QComboBox *cbNodes;
    QTextEdit *txtCurrentData;
    QLabel *lblSelected;
    QLabel *lblCurrentData;
    QTextEdit *txtLog;
    QLabel *lblLog;
    QPushButton *btnNextStep;
    QMenuBar *menubar;
    QMenu *menuAdd_note;
    QMenu *menuSimulate;
    QMenu *menuPresets;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setWindowModality(Qt::WindowModality::ApplicationModal);
        MainWindow->resize(1800, 1000);
        MainWindow->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        MainWindow->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #DAD6D6;\n"
"color: black;"));
        MainWindow->setAnimated(true);
        MainWindow->setTabShape(QTabWidget::TabShape::Rounded);
        actionRGB_Client_Node = new QAction(MainWindow);
        actionRGB_Client_Node->setObjectName("actionRGB_Client_Node");
        firstExample = new QAction(MainWindow);
        firstExample->setObjectName("firstExample");
        actionSimulate = new QAction(MainWindow);
        actionSimulate->setObjectName("actionSimulate");
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName("actionReset");
        actionArithmetic_Logic_Client_Node = new QAction(MainWindow);
        actionArithmetic_Logic_Client_Node->setObjectName("actionArithmetic_Logic_Client_Node");
        actionArithmetic_Logic_Client_Node->setCheckable(false);
        actionServer_Node = new QAction(MainWindow);
        actionServer_Node->setObjectName("actionServer_Node");
        secondExample = new QAction(MainWindow);
        secondExample->setObjectName("secondExample");
        thirdExample = new QAction(MainWindow);
        thirdExample->setObjectName("thirdExample");
        clientServerExample = new QAction(MainWindow);
        clientServerExample->setObjectName("clientServerExample");
        clientServerExample->setPriority(QAction::Priority::NormalPriority);
        actiontete = new QAction(MainWindow);
        actiontete->setObjectName("actiontete");
        fifthExample = new QAction(MainWindow);
        fifthExample->setObjectName("fifthExample");
        sixthExample = new QAction(MainWindow);
        sixthExample->setObjectName("sixthExample");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 800, 931, 181));
        frame->setStyleSheet(QString::fromUtf8("background-color: #B2B0B0;"));
        frame->setFrameShape(QFrame::Shape::Box);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        cbNodes = new QComboBox(frame);
        cbNodes->setObjectName("cbNodes");
        cbNodes->setGeometry(QRect(142, 20, 241, 21));
        cbNodes->setStyleSheet(QString::fromUtf8("background-color: #D9D9D9"));
        txtCurrentData = new QTextEdit(frame);
        txtCurrentData->setObjectName("txtCurrentData");
        txtCurrentData->setGeometry(QRect(142, 50, 241, 91));
        txtCurrentData->setStyleSheet(QString::fromUtf8("background-color: #D9D9D9"));
        lblSelected = new QLabel(frame);
        lblSelected->setObjectName("lblSelected");
        lblSelected->setGeometry(QRect(20, 20, 141, 21));
        lblSelected->setStyleSheet(QString::fromUtf8("background-color: transparent"));
        lblCurrentData = new QLabel(frame);
        lblCurrentData->setObjectName("lblCurrentData");
        lblCurrentData->setGeometry(QRect(20, 50, 141, 21));
        lblCurrentData->setStyleSheet(QString::fromUtf8("background-color: transparent"));
        txtLog = new QTextEdit(frame);
        txtLog->setObjectName("txtLog");
        txtLog->setGeometry(QRect(580, 20, 341, 111));
        txtLog->setStyleSheet(QString::fromUtf8("background-color: #D9D9D9"));
        lblLog = new QLabel(frame);
        lblLog->setObjectName("lblLog");
        lblLog->setGeometry(QRect(580, 0, 31, 21));
        lblLog->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        btnNextStep = new QPushButton(frame);
        btnNextStep->setObjectName("btnNextStep");
        btnNextStep->setGeometry(QRect(410, 20, 141, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1800, 22));
        menubar->setStyleSheet(QString::fromUtf8("QMenuBar {\n"
"background-color: #565151; \n"
"}\n"
"QMenu {\n"
"	background-color: #D9D9D9;\n"
"	color: white; \n"
"}\n"
"QMenu::item {\n"
"		color: black;\n"
"        background-color: #B1AFAF; \n"
"        padding: 8px 20px;            \n"
"    }\n"
"    QMenu::item:selected {           \n"
"        background-color: #555555;  \n"
"    }\n"
"    QMenu::separator {\n"
"        height: 1px;\n"
"        background-color: #444444;          \n"
"        margin: 4px 0px;\n"
"    }\n"
"\n"
"QAction {\n"
"	background-color: #B1AFAF;\n"
"}"));
        menuAdd_note = new QMenu(menubar);
        menuAdd_note->setObjectName("menuAdd_note");
        menuAdd_note->setAutoFillBackground(false);
        menuAdd_note->setStyleSheet(QString::fromUtf8("background-color: #B1AFAF;"));
        menuSimulate = new QMenu(menubar);
        menuSimulate->setObjectName("menuSimulate");
        menuSimulate->setStyleSheet(QString::fromUtf8("background-color: #B1AFAF;"));
        menuPresets = new QMenu(menubar);
        menuPresets->setObjectName("menuPresets");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuAdd_note->menuAction());
        menubar->addAction(menuSimulate->menuAction());
        menubar->addAction(menuPresets->menuAction());
        menuAdd_note->addAction(actionServer_Node);
        menuAdd_note->addAction(actionArithmetic_Logic_Client_Node);
        menuAdd_note->addAction(actionRGB_Client_Node);
        menuSimulate->addAction(actionSimulate);
        menuSimulate->addAction(actionReset);
        menuPresets->addAction(clientServerExample);
        menuPresets->addSeparator();
        menuPresets->addAction(firstExample);
        menuPresets->addAction(secondExample);
        menuPresets->addAction(thirdExample);
        menuPresets->addSeparator();
        menuPresets->addAction(sixthExample);
        menuPresets->addAction(fifthExample);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "CAN SIMULATOR - CODOREAN LUCA-ANDREI 30233 @2024", nullptr));
        actionRGB_Client_Node->setText(QCoreApplication::translate("MainWindow", "RGB Client Node", nullptr));
        firstExample->setText(QCoreApplication::translate("MainWindow", "1 ALC, 1 RGBCL 1 color swap", nullptr));
        actionSimulate->setText(QCoreApplication::translate("MainWindow", "Simulate", nullptr));
        actionReset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        actionArithmetic_Logic_Client_Node->setText(QCoreApplication::translate("MainWindow", "Arithmetic & Logic Client Node", nullptr));
        actionServer_Node->setText(QCoreApplication::translate("MainWindow", "Server Node", nullptr));
        secondExample->setText(QCoreApplication::translate("MainWindow", "1 ALC, 1 RGBCL, 2 color swaps", nullptr));
        thirdExample->setText(QCoreApplication::translate("MainWindow", "1 ALC, 2 RGBCL, 1 color swap each", nullptr));
        clientServerExample->setText(QCoreApplication::translate("MainWindow", "Client-Server Communication", nullptr));
        actiontete->setText(QCoreApplication::translate("MainWindow", "tete", nullptr));
        fifthExample->setText(QCoreApplication::translate("MainWindow", "Complex communication example", nullptr));
        sixthExample->setText(QCoreApplication::translate("MainWindow", "Invalid destination error catcher", nullptr));
        lblSelected->setText(QCoreApplication::translate("MainWindow", "Selected node:", nullptr));
        lblCurrentData->setText(QCoreApplication::translate("MainWindow", "Current frame details:", nullptr));
        lblLog->setText(QCoreApplication::translate("MainWindow", "Log:", nullptr));
        btnNextStep->setText(QCoreApplication::translate("MainWindow", "Step in", nullptr));
        menuAdd_note->setTitle(QCoreApplication::translate("MainWindow", "Add note", nullptr));
        menuSimulate->setTitle(QCoreApplication::translate("MainWindow", "Options", nullptr));
        menuPresets->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #B1AFAF;", nullptr));
        menuPresets->setTitle(QCoreApplication::translate("MainWindow", "Presets", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
