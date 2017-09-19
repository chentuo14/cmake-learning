#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("右键菜单显示"));
    //为窗口添加QAction
    addAction(new QAction(tr("&Opne"), this));
    addAction(new QAction(tr("&Opt"), this));
    addAction(new QAction(tr("&Quit"), this));
    //设置contexMenuPolicy属性值为，以Actions为弹出菜单的菜单项组成菜单
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

Widget::~Widget()
{
    delete ui;
}
