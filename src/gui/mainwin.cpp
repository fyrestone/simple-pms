#include "mainwin.h"
#include "mainwin_p.h"
#include "ui_mainwin.h"
#include "../context/context.h"

MainWinPrivate::MainWinPrivate()
    :task(DataEngine::Task::instance())
{
    QIcon addTabIcon, delTabIcon;

    addTabIcon.addFile(QString(QObject::tr(":/Icon/image/add.png")), QSize(), QIcon::Normal, QIcon::Off);
    addTabButton.setIcon(addTabIcon);
    addTabButton.setFlat(true);

    delTabIcon.addFile(QString(QObject::tr(":/Icon/image/delete.png")), QSize(), QIcon::Normal, QIcon::Off);
    delTabButton.setIcon(delTabIcon);
    delTabButton.setFlat(true);

    connect(task, SIGNAL(finished(int,QVariant)),
            this, SLOT(finished(int,QVariant)));
}

void MainWinPrivate::finished(int taskID, const QVariant &result)
{
    switch(taskID)
    {
    case DataEngine::FillNavigationTree:
        if(result.type() == QVariant::Bool && result.toBool())
            emit expandClassTree();
    }
}

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin),
    d(new MainWinPrivate)
{
    ui->setupUi(this);

    setCustomApperance();
    connectSignalsAndSlots();
}

MainWin::~MainWin()
{
    delete ui;
    delete d;
}

void MainWin::initializeAndShow()
{
    initializeAll();
    show();
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

void MainWin::testSlot()
{
    d->task->insertOrUpdateNavigationTree(ui->navigationTree, 2006, 2, "fuck");
}

void MainWin::initializeAll()
{
    d->task->fillNavigationTree(ui->navigationTree, tr("驻马店第一初级中学"));
}

void MainWin::setCustomApperance()
{
    ui->centralTabwidget->setCornerWidget(&d->addTabButton, Qt::TopLeftCorner);
    ui->centralTabwidget->setCornerWidget(&d->delTabButton, Qt::TopRightCorner);
    ui->centralTabwidget->addTab(new QFrame(), QString("fuck"));
}

void MainWin::connectSignalsAndSlots()
{
    connect(ui->aboutAct, SIGNAL(triggered()), &d->aboutDlg, SLOT(exec()));
    connect(ui->classMgmtAct, SIGNAL(triggered()), &d->classMgmtDlg, SLOT(exec()));
    connect(d, SIGNAL(expandClassTree()), this, SLOT(testSlot()));
}
