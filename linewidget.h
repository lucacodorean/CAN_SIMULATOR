#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QWidget>
#include <QEvent>
class LineWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit LineWidget(QWidget *parent);
        explicit LineWidget(QWidget*, QWidget*, QWidget*);
        explicit LineWidget(QWidget*, QWidget*, QWidget*, int);

    protected:
        void paintEvent(QPaintEvent*);

        bool eventFilter(QObject *watched, QEvent *event) override {
            if ((watched == from || watched == to) &&
                (event->type() == QEvent::Move || event->type() == QEvent::Resize)) {
                adjustGeometry();
                update();
            }

            update();
            adjustGeometry();
            return QWidget::eventFilter(watched, event);
        }


    private:
        QWidget* from;
        QWidget* to;

        int position;

        void adjustGeometry() {
            if (parentWidget()) {
                setGeometry(parentWidget()->rect());
            }
        }
};

#endif // LINEWIDGET_H
