#ifndef CAN_NODE_H
#define CAN_NODE_H

#include <QWidget>
#include "draggable.h"
#include "NodeType.h"

namespace Ui {
class CAN_NODE;
}

class CAN_NODE : public Draggable
{
    Q_OBJECT

    public:
        explicit CAN_NODE(QWidget *parent = nullptr);
        explicit CAN_NODE(QString label, NodeType type, QString content, QWidget *parent = nullptr);
        ~CAN_NODE();

        Ui::CAN_NODE *ui;

        void updateFrameHistory(std::string);

    private:
};

#endif // CAN_NODE_H
