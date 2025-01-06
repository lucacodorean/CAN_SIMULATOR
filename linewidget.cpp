#include "linewidget.h"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>


LineWidget::LineWidget(QWidget *parent) : QWidget{parent} {}
LineWidget::LineWidget(QWidget* from, QWidget* to, QWidget *parent) : QWidget{parent}, from{from}, to{to} {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    adjustGeometry();
    this->position = 0;
}
LineWidget::LineWidget(QWidget* from, QWidget* to, QWidget *parent, int position) : QWidget{parent}, from{from}, to{to} {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    adjustGeometry();
    this->position = position;
}

void LineWidget::paintEvent(QPaintEvent *event){
    if (!this->from || !this->to) return;

    QPainter painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 4));

    if(position == 1) {
        QPoint point1 = this->from->geometry().topRight();
        QPoint point2 = this->to->geometry().topLeft();
        painter.drawLine(point1, point2);
    }
    else {
        QPoint point1 = this->from->geometry().bottomRight();
        QPoint point2 = this->to->geometry().bottomLeft();
        painter.drawLine(point1, point2);
    }

    adjustGeometry();
    update();
}

