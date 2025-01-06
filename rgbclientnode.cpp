#include "rgbclientnode.h"
#include "ui_can_node.h"

RGBClientNode::RGBClientNode() : ClientNode() { }

RGBClientNode::RGBClientNode(DWORD identifier, QString label, QString content, QWidget* parent)
    : ClientNode(identifier, label, NodeType::RGBClientNode, content, parent) {}

BYTE decodeData(DWORD data, unsigned int index) {
    return (data >> (8*index)) & 0xff;
}

BOOL RGBClientNode::handleFrame(Frame* frame, INode* destinationNode) {
    Frame* topFrame = this->getTopFrame();
    if(topFrame == nullptr || topFrame->getPriority() == 0) return false;

    BYTE RED =      decodeData(topFrame->getData(), 2);
    BYTE GREEN =    decodeData(topFrame->getData(), 1);
    BYTE BLUE =     decodeData(topFrame->getData(), 0);

    qDebug() <<"RGB:"<< RED <<" "<<GREEN<<" "<<BLUE;

    QString style = QString("background-color: rgb(%1, %2, %3);")
                        .arg(RED).arg(GREEN).arg(BLUE);

    this->ui->content->setStyleSheet(style);

    Frame* temp = new Frame(this->getIndentifier(), destinationNode->getIndentifier(), 0x01, 0xFF, 0x11, 0x1);

    this->insertFrame(temp);

    std::string tempString = "W: " + temp->toPipeStructure().toStdString();
    CAN_NODE* myNode = dynamic_cast<CAN_NODE*>(this);
    myNode->updateFrameHistory(tempString);

    update();

    return true;
}
