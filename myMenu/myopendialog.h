#ifndef MYOPENDIALOG_H
#define MYOPENDIALOG_H

#include <QDialog>

namespace Ui {
class MyOpenDialog;
}

class MyOpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyOpenDialog(QWidget *parent = 0);
    ~MyOpenDialog();

private:
    Ui::MyOpenDialog *ui;
};

#endif // MYOPENDIALOG_H
