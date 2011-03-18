#include "mainwin.h"
#include "mainwin_p.h"
#include "ui_mainwin.h"
#include "../gui/custom/navigationitem.h"
#include "../context/context.h"

MainWinPrivate::MainWinPrivate(MainWin *parent) :
    task(DataEngine::Task::instance()),
    q(parent),
    aboutDlg(parent),
    addGradeWizard(parent),
    addClassWizard(parent)
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

    connect(addGradeAct, SIGNAL(triggered()), &addGradeWizard, SLOT(open()));
    connect(addClassAct, SIGNAL(triggered()), &addClassWizard, SLOT(open()));

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
    task->lookup<DataEngine::FillNavigationTreeTask>()->run(q->ui->navigationTree, tr("驻马店第一初级中学"));
}

void MainWinPrivate::finished(int taskID, const QVariant &result)
{
    switch(taskID)
    {
    case DataEngine::InsertOrUpdateClass:
        if(result.type() == QVariant::Bool && result.toBool())
            task->lookup<DataEngine::FillNavigationTreeTask>()->asyncRun(q->ui->navigationTree, tr("驻马店第一初级中学"));
        break;
    }
}

void MainWinPrivate::showNavigationContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = q->ui->navigationTree->itemAt(pos);

    if(item)
    {
        switch(item->type())
        {
        case NavigationItem::Root:
            rootContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
        case NavigationItem::Grade:
            gradeContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
        case NavigationItem::Class:
            classContextMenu.popup(q->ui->navigationTree->mapToGlobal(pos));
            break;
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
