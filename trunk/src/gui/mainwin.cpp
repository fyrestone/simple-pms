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
    QAction *mgmtClassAct = new QAction(tr("管理学生"), this);

    connect(addGradeAct, SIGNAL(triggered()), this, SLOT(showAddGradeWizard()));
    connect(addClassAct, SIGNAL(triggered()), this, SLOT(showAddClassWizard()));
    connect(delGradeAct, SIGNAL(triggered()), this, SLOT(deleteGradeClass()));
    connect(delClassAct, SIGNAL(triggered()), this, SLOT(deleteGradeClass()));
    connect(mgmtClassAct, SIGNAL(triggered()), this, SLOT(showClassMgmtDlg()));

    rootContextMenu.addAction(addGradeAct);
    gradeContextMenu.addAction(addClassAct);
    gradeContextMenu.addAction(delGradeAct);
    classContextMenu.addAction(mgmtClassAct);
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

void MainWinPrivate::showClassMgmtDlg()
{
    QList<QTreeWidgetItem *> selectedItems = q->ui->navigationTree->selectedItems();

    if(!selectedItems.isEmpty())
    {
        QTreeWidgetItem *classItem = selectedItems[0];                         //待删除节点指针
        QTreeWidgetItem *gradeItem = classItem->parent();

        if(classItem->type() == DataEngine::Class && gradeItem->type() == DataEngine::Grade)
        {
            int mgmtGradeNum = gradeItem->data(0, Qt::UserRole).toInt();
            int mgmtClassNum = classItem->data(0, Qt::UserRole).toInt();

            ClassMgmtDlg classMgmtDlg(mgmtGradeNum, mgmtClassNum, q);

            classMgmtDlg.exec();
        }
    }
}

void MainWinPrivate::deleteGradeClass()
{
    QList<QTreeWidgetItem *> selectedItems = q->ui->navigationTree->selectedItems();

    if(!selectedItems.isEmpty())
    {
        static const QString requestComformTemplate = tr("确定删除 %1 ？");      //请求确认字符串模板
        QTreeWidgetItem *deleteItem = selectedItems[0];                         //待删除节点指针
        QString requestComformStr;                                              //请求确认字符串
        int deleteGradeNum = INVALID_GRADE_NUM;                                 //待删除年级号
        int deleteClassNum = INVALID_CLASS_NUM;                                 //待删除班级号

        /* 根据待删除节点类型构建请求确认字符串 */
        switch(deleteItem->type())
        {
        case DataEngine::Root:  //根节点
            break;
        case DataEngine::Grade: //年级节点
            {
                deleteGradeNum = deleteItem->data(0, Qt::UserRole).toInt();
                QString gradeStr = QString::number(deleteGradeNum) + tr("级");
                requestComformStr = requestComformTemplate.arg(gradeStr);
            }
            break;
        case DataEngine::Class: //班级节点
            {
                QTreeWidgetItem *gradeParent = deleteItem->parent();

                if(gradeParent)
                {
                    deleteGradeNum = gradeParent->data(0, Qt::UserRole).toInt();
                    deleteClassNum = deleteItem->data(0, Qt::UserRole).toInt();
                    QString gradeStr = QString::number(deleteGradeNum) + tr("年级");
                    QString classStr = QString::number(deleteClassNum) + tr("班");
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
            {
                QPointer<QTreeWidget> navigationTree = q->ui->navigationTree;

                task->lookup<DataEngine::DeleteClassTask>()->run(deleteGradeNum, deleteClassNum);
                task->lookup<DataEngine::FillNavigationTreeTask>()->asyncRun(
                            navigationTree, tr("驻马店第一初级中学"));
            }
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
