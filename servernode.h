#ifndef SERVERNODE_H
#define SERVERNODE_H

#include "INode.h"
#include "can_node.h"

class ServerNode : public INode, public CAN_NODE
{
    public:
        ServerNode();
        ServerNode(DWORD, QWidget*);
        ServerNode(DWORD, QString, QString, QWidget*);

        BOOL handleFrame(Frame*, INode* = nullptr);
};

#endif // SERVERNODE_H
