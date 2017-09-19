#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MdiChild;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateMenus();         //更新菜单
    MdiChild *createMdiChild(); //创建子窗口

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QAction *actionSeparator;       //间隔器
    MdiChild *activeMdiChild();     //活动窗口
};

#endif // MAINWINDOW_H
