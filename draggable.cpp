#include "draggable.h"

Draggable::Draggable(QWidget *parent) : QWidget{parent} {
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
}


void Draggable::mousePressEvent(QMouseEvent *event)  {
    this->offset = event->pos();
}

void Draggable::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        this->move(this->pos() + event->pos() - this->offset);
    }
}
