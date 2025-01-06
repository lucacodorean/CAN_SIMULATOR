#include "servernode.h"
#include "alclientnode.h"
#include "rgbclientnode.h"
#include <QMessageBox>

ServerNode::ServerNode() : INode(0), CAN_NODE(nullptr) {}

ServerNode::ServerNode(DWORD identifier, QWidget* parent = nullptr) : INode(identifier), CAN_NODE(parent) {}

ServerNode::ServerNode(DWORD identifier,QString label, QString content, QWidget* parent)
    : INode(identifier), CAN_NODE(label, NodeType::ServerNode, content, parent) { }

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

BOOL ServerNode::handleFrame(Frame* frame, INode* destinationNode) {


    if(destinationNode->getIndentifier() != frame->getDestinationNode()) {
        Frame* errorFrame = new Frame(frame->getOriginNode(), frame->getDestinationNode(), 0x00, 0x7FFFFFF, 11, 9);
        this->setFrame(*errorFrame);
        this->insertFrame(errorFrame);
        return false;
    }

    std::string tempString = "W: " + frame->toPipeStructure().toStdString();
    CAN_NODE* myNode = dynamic_cast<CAN_NODE*>(this);
    myNode->updateFrameHistory(tempString);

    BOOL ok = false;

    if(instanceof<ALClientNode>(destinationNode))  ok = frame->getData() <= 0x7FFFFFF;
    if(instanceof<RGBClientNode>(destinationNode)) ok = frame->getData() <= 0xFFFFFF;

    if(ok) {
        this->setFrame(*frame);
        this->insertFrame(frame);
        return true;
    }
    return false;
}
