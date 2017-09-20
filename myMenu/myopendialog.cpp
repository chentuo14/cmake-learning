#include "myopendialog.h"
#include "ui_myopendialog.h"

MyOpenDialog::MyOpenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyOpenDialog)
{
    ui->setupUi(this);
}

MyOpenDialog::~MyOpenDialog()
{
    delete ui;
}
