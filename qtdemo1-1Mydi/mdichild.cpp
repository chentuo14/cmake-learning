#include "mdichild.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>
#include <QFileDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QMenu>

MdiChild::MdiChild(QWidget *parent)
{
    //设置在子窗口关闭时销毁这个类的对象
    setAttribute(Qt::WA_DeleteOnClose);

    //初始isUntitled为true
    isUntitled = true;
}

//新建文件
void MdiChild::newFile()
{
    //设置窗口编号，因为编号一直被保存，所以需要使用静态变量
    static int sequenceNumber = 1;

    //新建的文档没有被保存过
    isUntitled = true;

    //将当前文件命名为未命名文档加编号，编号先试用再加1
    curFile = tr("未命名文档%1.txt").arg(sequenceNumber++);

    //设置窗口标题，使用[*]可以再文档被更改后在文件名称后显示"*"号
    setWindowTitle(curFile + "[*]" + tr(" - 多文档编辑器"));

    //文档更改时发射contentsChanged()信号，执行documentWasModified()槽
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

}

void MdiChild::documentWasModified()
{
    //根据文档的isModified()函数的返回值，判断编辑器内容是否被更改了
    //如果被更改了，就要在设置了[*]号的地方显示“*”号，这里会在窗口标题中显示
    setWindowModified(document()->isModified());
}

bool MdiChild::loadFile(const QString &fileName)
{
    //新建QFile对象
    QFile file(fileName);
    //只读方式打开文件，出错则提示，并返回false
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"), tr("无法读取文件%1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    //新建文本流对象
    QTextStream in(&file);
    //设置鼠标状态为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //读取文件的全部文本内容，并添加到编辑器中
    setPlainText(in.readAll());
    //恢复鼠标状态
    QApplication::restoreOverrideCursor();
    //设置当前文件
    setCurrentFile(fileName);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    //canonicalFilePath()可以出去路径中的符号链接，“.”和“..”等符号
    curFile = QFileInfo(fileName).canonicalFilePath();

    //文件已经被保存过了
    isUntitled = false;

    //文档没有被更改过
    document()->setModified(false);

    //窗口不显示更改标志
    setWindowModified(false);

    //设置窗口标题,userFriendlyCurrentFile()返回文件名
    setWindowTitle(userFriendlyCurrentFile()+"[*]");
}

QString MdiChild::userFriendlyCurrentFile()
{
    //从文件路径中提取文件名
    return QFileInfo(curFile).fileName();
}

bool MdiChild::save()
{
    //如果文件未被保存过，则执行另存为操作，否则直接保存文件
    if(isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), curFile);
    //获取文件路径，如果为空，则返回false,否则保存文件
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"), tr("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //以村文本文件写入
    out<<toPlainText();
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    return true;
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    //如果maybeSave()函数返回true,则关闭窗口,否则忽略该事件
    if(maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::contextMenuEvent(QContextMenuEvent *e)
{
    //创建菜单，并向其中添加动作
    QMenu *menu = new QMenu;
    QAction *undo = menu->addAction(tr("撤销(&U)"), this,
                                    SLOT(undo()), QKeySequence::Undo);
    undo->setEnabled(document()->isUndoAvailable());
    QAction *redo = menu->addAction(tr("恢复(&R)"), this,
                                    SLOT(redo()), QKeySequence::Redo);
    redo->setEnabled(document()->isRedoAvailable());
    menu->addSeparator();
    QAction *cut = menu->addAction(tr("剪切(&T)"), this,
                                   SLOT(cut()), QKeySequence::Cut);
    cut->setEnabled(textCursor().hasSelection());
    QAction *copy = menu->addAction(tr("复制(&C)"), this,
                                    SLOT(copy()), QKeySequence::Copy);
    copy->setEnabled(textCursor().hasSelection());
    menu->addAction(tr("粘贴(&P)"), this, SLOT(paste()), QKeySequence::Paste);
    QAction *clear = menu->addAction(tr("清空"), this, SLOT(clear()));
    clear->setEnabled(!document()->isEmpty());
    menu->addSeparator();
    QAction *select = menu->addAction(tr("全选"), this,
                                      SLOT(selectAll()), QKeySequence::SelectAll);
    select->setEnabled(!document()->isEmpty());

    //获取鼠标的位置，然后再这个位置显示菜单
    menu->exec(e->globalPos());

    //最后销毁这个菜单
    delete menu;
}

bool MdiChild::maybeSave()
{
    //如果文档被更改过
    if(document()->isModified()) {
        QMessageBox box;
        box.setWindowTitle(tr("多文档编辑器"));
        box.setText(tr("是否保存对“%1”的更改？").arg(userFriendlyCurrentFile()));
        box.setIcon(QMessageBox::Warning);

        //添加按钮，QMessageBox::YesRole可以表明这个按钮的行为
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"), QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("取消"), QMessageBox::RejectRole);
        //弹出对话框，让用户选择是否保存修改，或者取消关闭操作
        box.exec();
        //如果用户选择是,则返回保存操作的结果；乳沟选择取消，则返回false
        if(box.clickedButton() == yesBtn)
            return save();
        else if(box.clickedButton() == cancelBtn)
            return false;
    }

    //如果文档没有更改过，则直接返回true
    return true;
}
