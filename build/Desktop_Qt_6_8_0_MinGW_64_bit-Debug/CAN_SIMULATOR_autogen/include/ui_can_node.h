/********************************************************************************
** Form generated from reading UI file 'can_node.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAN_NODE_H
#define UI_CAN_NODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAN_NODE
{
public:
    QLabel *description;
    QLabel *content;
    QLabel *frameHistory;

    void setupUi(QWidget *CAN_NODE)
    {
        if (CAN_NODE->objectName().isEmpty())
            CAN_NODE->setObjectName("CAN_NODE");
        CAN_NODE->resize(307, 168);
        CAN_NODE->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        description = new QLabel(CAN_NODE);
        description->setObjectName("description");
        description->setGeometry(QRect(20, 120, 101, 16));
        description->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        description->setAlignment(Qt::AlignmentFlag::AlignCenter);
        content = new QLabel(CAN_NODE);
        content->setObjectName("content");
        content->setGeometry(QRect(20, 20, 101, 71));
        content->setStyleSheet(QString::fromUtf8("border: 1px solid black"));
        content->setScaledContents(true);
        frameHistory = new QLabel(CAN_NODE);
        frameHistory->setObjectName("frameHistory");
        frameHistory->setGeometry(QRect(170, 10, 131, 141));
        frameHistory->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        frameHistory->setAlignment(Qt::AlignmentFlag::AlignCenter);

        retranslateUi(CAN_NODE);

        QMetaObject::connectSlotsByName(CAN_NODE);
    } // setupUi

    void retranslateUi(QWidget *CAN_NODE)
    {
        CAN_NODE->setWindowTitle(QCoreApplication::translate("CAN_NODE", "Form", nullptr));
        description->setText(QCoreApplication::translate("CAN_NODE", "TextLabel", nullptr));
        content->setText(QString());
        frameHistory->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CAN_NODE: public Ui_CAN_NODE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAN_NODE_H
