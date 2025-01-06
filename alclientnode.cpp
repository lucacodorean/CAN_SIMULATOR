#include "alclientnode.h"
#include "OperationType.h"

#include <qDebug>
#include <QString>

ALClientNode::ALClientNode() : ClientNode() { }

ALClientNode::ALClientNode(DWORD identifier, QString label, QString content, QWidget* parent)
    : ClientNode(identifier, label, NodeType::ALClientNode, content, parent) {}


BYTE decodeOperation(DWORD data) {
    return (data >> 24) & 0xff;
}

BYTE getOperand(DWORD data, unsigned int index) {
    return (data >> (8*index)) & 0xff;
}

DWORD computeResult(BYTE operationType, BYTE first_operand, BYTE second_operand, BYTE third_operand) {
    int status = 0;

    switch (operationType) {
    case BYTE(OperationType::ADDITION):
        status = (first_operand + second_operand + third_operand);
        break;
    case BYTE(OperationType::SUBTRACT):
        status = (first_operand + second_operand - third_operand);
        break;
    case BYTE(OperationType::MULTIPLICATION):
        status = (first_operand * second_operand * third_operand);
        break;
    case BYTE(OperationType::DIVISION):
        status = (first_operand / second_operand / third_operand);
        break;
    case BYTE(OperationType::AND):
        status = (first_operand & second_operand & third_operand);
        break;
    case BYTE(OperationType::OR):
        status = (first_operand | second_operand | third_operand);
        break;
    case BYTE(OperationType::XOR):
        status = (first_operand ^ second_operand ^ third_operand);
        break;
    case BYTE(OperationType::NAND):
        status = ~(first_operand & second_operand & third_operand);
        break;
    default:
        break;
    }


    if(status < 0) status = 0;
    return status;
}

BOOL ALClientNode::handleFrame(Frame* frame, INode* destinationNode) {
    Frame* topFrame = this->getTopFrame();
    DWORD frameData = topFrame->getData();

    BYTE operationType = decodeOperation(frameData);
    if(operationType > 0x07) return false;

    BYTE firstOperand  = getOperand(frameData, 2);
    BYTE secondOperand = getOperand(frameData, 1);
    BYTE thirdOperand  = getOperand(frameData, 0);
    DWORD status = computeResult(operationType, firstOperand, secondOperand, thirdOperand);

    Frame* temp = new Frame(this->getIndentifier(), destinationNode->getIndentifier(), 0x01, status, 0x11, 0x1);

    this->insertFrame(temp);

    std::string tempString = "W: " + temp->toPipeStructure().toStdString();
    CAN_NODE* myNode = dynamic_cast<CAN_NODE*>(this);
    myNode->updateFrameHistory(tempString);
    update();


    return true;
}
