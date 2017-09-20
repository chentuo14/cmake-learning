#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("右键菜单显示"));
    QAction *openAct = new QAction(tr("&open"),this);
    openAct->setStatusTip("Open a file");
    //为窗口添加QAction
    addAction(openAct);
    addAction(new QAction(tr("&Opt"), this));
    addAction(new QAction(tr("&Quit"), this));
    //设置contexMenuPolicy属性值为，以Actions为弹出菜单的菜单项组成菜单
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(openAction()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openAction()
{
    qDebug()<<"this is open";
}
