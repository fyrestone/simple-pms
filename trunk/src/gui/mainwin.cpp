#include "mainwin.h"
#include "mainwin_p.h"
#include "ui_mainwin.h"
#include "../context/context.h"

#include <QMessageBox>

MainWinPrivate::MainWinPrivate(MainWin *parent) :
    task(DataEngine::Task::instance()),
    q(parent),
    aboutDlg(parent)
{
}

void MainWinPrivate::initializeMember()
{
    QIcon addTabIcon, delTabIcon;

    addTabIcon.addFile(QString(QObject::tr(":/Icon/image/add.png")), QSize(), QIcon::Normal, QIcon::Off);
    addTabButton.setIcon(addTabIcon);
    addTabButton.setFlat(true);

    delTabIcon.addFile(QString(QObject::tr(":/Icon/image/delete.png")), QSize(), QIcon::Normal, QIcon::Off);
    delTabButton.setIcon(delTabIcon);
    delTabButton.setFlat(true);

    QAction *addGradeAct = new QAction(tr("添加年级"), this);
    QAction *delGradeAct = new QAction(tr("删除年级"), this);
    QAction *addClassAct = new QAction(tr("添加班级"), this);
    QAction *delClassAct = new QAction(tr("删除班级"), this);
    QAction *modClassTypeAct = new QAction(tr("修改班级类型"), this);

    connect(addGradeAct, SIGNAL(triggered()), this, SLOT(showAddGradeWizard()));
    connect(addClassAct, SIGNAL(triggered()), this, SLOT(showAddClassWizard()));
    connect(delGradeAct, SIGNAL(triggered()), this, SLOT(deleteGradeClass()));
    connect(delClassAct, SIGNAL(triggered()), this, SLOT(deleteGradeClass()));

    rootContextMenu.addAction(addGradeAct);
    gradeContextMenu.addAction(addClassAct);
    gradeContextMenu.addAction(delGradeAct);
    classContextMenu.addAction(modClassTypeAct);
    classContextMenu.addAction(delClassAct);

    q->ui->centralTabwidget->setCornerWidget(&addTabButton, Qt::TopLeftCorner);
    q->ui->centralTabwidget->setCornerWidget(&delTabButton, Qt::TopRightCorner);
    q->ui->centralTabwidget->addTab(new QFrame(), QString("fuck"));
}

void MainWinPrivate::connectSignalsAndSlots()
{
    connect(task,                   SIGNAL(finished(int,QVariant)),
            this,                   SLOT(finished(int,QVariant)));
    connect(q->ui->navigationTree,  SIGNAL(customContextMenuRequested(QPoint)),
            this,                   SLOT(showNavigationContextMenu(QPoint)));
    connect(q->ui->aboutAct,        SIGNAL(triggered()),
            &aboutDlg,              SLOT(exec()));
}

void MainWinPrivate::completeConstruct()
{
    task->lookup<DataEngine::FillNavigationTreeTask>()->run(
                q->ui->navigationTree, tr("驻马店第一初级中学"));
}

void MainWinPrivate::finished(int taskID, const QVariant &result)
{

}

void MainWinPrivate::showNavigationContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = q->ui->navigationTree->itemAt(pos);

    if(item)
    {
        switch(item->type())
        {
        case DataEngine::Root:
            rootContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
        case DataEngine::Grade:
            gradeContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
        case DataEngine::Class:
            classContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
        }

        qDebug() << item->data(0, Qt::UserRole);
    }
}

void MainWinPrivate::showAddGradeWizard()
{
    AddGradeWizard addGradeWizard(q);

    if(addGradeWizard.exec() == QDialog::Accepted)
    {
        QPointer<QTreeWidget> navigationTree = q->ui->navigationTree;

        task->lookup<DataEngine::FillNavigationTreeTask>()->asyncRun(
                    navigationTree, tr("驻马店第一初级中学"));
    }
}

void MainWinPrivate::showAddClassWizard()
{
    AddClassWizard addClassWizard(q);

    if(addClassWizard.exec() == QDialog::Accepted)
    {
        QPointer<QTreeWidget> navigationTree = q->ui->navigationTree;

        task->lookup<DataEngine::FillNavigationTreeTask>()->asyncRun(
                    navigationTree, tr("驻马店第一初级中学"));
    }
}

void MainWinPrivate::deleteGradeClass()
{
    QList<QTreeWidgetItem *> selectedItems = q->ui->navigationTree->selectedItems();

    if(!selectedItems.isEmpty())
    {
        static const QString requestComformTemplate = tr("确定删除 %1 ？");   //请求确认字符串模板
        QString requestComformStr;                                          //请求确认字符串
        QTreeWidgetItem *deleteItem = selectedItems[0];                     //待删除节点指针

        /* 根据待删除节点类型构建请求确认字符串 */
        switch(deleteItem->type())
        {
        case DataEngine::Root:  //根节点
            //空，不允许删除根
            break;
        case DataEngine::Grade: //年级节点
            {
                QString gradeStr = deleteItem->data(0, Qt::UserRole).toString() + tr("级");
                requestComformStr = requestComformTemplate.arg(gradeStr);
            }
            break;
        case DataEngine::Class: //班级节点
            {
                QTreeWidgetItem *gradeParent = deleteItem->parent();

                if(gradeParent)
                {
                    QString gradeStr = gradeParent->data(0, Qt::UserRole).toString() + tr("年级");
                    QString classStr = deleteItem->data(0, Qt::UserRole).toString() + tr("班");

                    requestComformStr = requestComformTemplate.arg(gradeStr + classStr);
                }
            }
            break;
        }

        /* 若构建请求确认字符串成功，弹出请求确认对话框 */
        if(!requestComformStr.isEmpty())
        {
            QMessageBox requestComformDlg(q);

            requestComformDlg.setWindowTitle(tr("确认删除"));
            requestComformDlg.setText(requestComformStr);
            requestComformDlg.addButton(tr("删除"), QMessageBox::AcceptRole);
            requestComformDlg.addButton(tr("取消"), QMessageBox::RejectRole);
            requestComformDlg.setIcon(QMessageBox::Question);

            if(requestComformDlg.exec() == QMessageBox::AcceptRole)
                qDebug() << "Accepted!";
            else
                qDebug() << "Canceled!";
        }
    }
}

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin),
    d(new MainWinPrivate(this))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
    d->completeConstruct();
}

MainWin::~MainWin()
{
    delete ui;
    delete d;
}

void MainWin::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
