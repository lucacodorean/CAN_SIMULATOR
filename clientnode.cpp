#include "clientnode.h"

ClientNode::ClientNode() : INode(0), CAN_NODE(nullptr) {}

ClientNode::ClientNode(DWORD identifier, QWidget* parent = nullptr) : INode(identifier), CAN_NODE(parent) {}

ClientNode::ClientNode(DWORD identifier, QString label, NodeType type, QString content, QWidget* parent)
    : INode(identifier), CAN_NODE(label, type, content, parent)
{

}
