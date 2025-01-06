#ifndef RGBCLIENTNODE_H
#define RGBCLIENTNODE_H

#include "clientnode.h"
#include <QWidget>

class RGBClientNode : public ClientNode {

public:
    RGBClientNode();
    RGBClientNode(DWORD, QString, QString, QWidget*);

    BOOL handleFrame(Frame*, INode* = nullptr);
};

#endif // RGBCLIENTNODE_H
