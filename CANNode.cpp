#include "../CANNode.h"
#include "ui_CANNode.h"

temp::temp(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::temp)
{
    ui->setupUi(this);
}

temp::~temp()
{
    delete ui;
}
