#ifndef DRAGGABLE_H
#define DRAGGABLE_H

    #include <QLabel>
    #include <QPoint>
    #include <QMouseEvent>
    #include <QVBoxLayout>

    class Draggable : public QWidget {
        Q_OBJECT

    public:
        explicit Draggable(QWidget *parent);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

    private:
        QPoint offset;
    };

#endif
