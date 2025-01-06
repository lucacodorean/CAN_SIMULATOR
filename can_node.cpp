#include "can_node.h"
#include "ui_can_node.h"

#include <QDebug>

CAN_NODE::CAN_NODE(QWidget *parent) : Draggable(parent), ui(new Ui::CAN_NODE) {
    ui->setupUi(this);
}

CAN_NODE::CAN_NODE(QString label, NodeType type, QString content, QWidget *parent) : Draggable(parent), ui(new Ui::CAN_NODE) {

    ui->setupUi(this);
    ui->description->setText(label);

    if(type == NodeType::ALClientNode || type == NodeType::ServerNode) {
        QPixmap temp = QPixmap(content);
        ui->content->setPixmap(temp);
        ui->content->setFixedSize(100, 100);
    }

    else if(type == NodeType::RGBClientNode) {
        ui->content->setStyleSheet("background-color: white; padding: 5px;");
        ui->content->setFixedSize(100, 100);
    }
}

void CAN_NODE::updateFrameHistory(std::string data) {
    QString text = this->ui->frameHistory->text();    
    text.append("\n"); text.append(data);
    this->ui->frameHistory->setText(text);
}

CAN_NODE::~CAN_NODE() {
    delete ui;
}
