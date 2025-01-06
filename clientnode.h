#ifndef CLIENTNODE_H
#define CLIENTNODE_H

#include "INode.h"
#include "NodeType.h"
#include "can_node.h"


class ClientNode : public INode, public CAN_NODE
{

public:
    ClientNode();
    ClientNode(DWORD, QWidget*);
    ClientNode(DWORD, QString, NodeType, QString, QWidget*);

    virtual BOOL handleFrame(Frame*, INode* = nullptr) = 0;

};

#endif // CLIENTNODE_H
