#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myopendialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void openAction();

private:
    Ui::Widget *ui;
    MyOpenDialog *myOpenDialog;
};

#endif // WIDGET_H
