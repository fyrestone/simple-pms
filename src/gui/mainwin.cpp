#include "mainwin.h"
#include "mainwin_p.h"
#include "ui_mainwin.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new MainWinPrivate(this)),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);

    setCustomApperance();
    connectSignalsAndSlots();
}

MainWin::~MainWin()
{
    delete ui;
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

void MainWin::setCustomApperance()
{
    Q_D(MainWin);

    ui->centralTabwidget->setCornerWidget(&d->addTabButton, Qt::TopLeftCorner);
    ui->centralTabwidget->setCornerWidget(&d->delTabButton, Qt::TopRightCorner);
    ui->centralTabwidget->addTab(new QFrame(), QString("fuck"));
}

void MainWin::connectSignalsAndSlots()
{
    Q_D(MainWin);

    connect(ui->aboutAct, SIGNAL(triggered()), &d->aboutDlg, SLOT(exec()));
}
