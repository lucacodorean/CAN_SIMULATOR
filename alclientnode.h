#ifndef ALCLIENTNODE_H
#define ALCLIENTNODE_H

#include "clientnode.h"

class ALClientNode : public ClientNode {

public:
    ALClientNode();
    ALClientNode(DWORD,QString, QString, QWidget*);

    BOOL handleFrame(Frame*, INode*);
};

#endif // ALCLIENTNODE_H
