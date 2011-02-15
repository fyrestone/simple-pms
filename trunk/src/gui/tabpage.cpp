#include "tabpage.h"
#include "ui_tabpage.h"
#include "logic/tabpage_p.h"

TabPage::TabPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TabPage),
    d_ptr(new TabPagePrivate(this))
{
    ui->setupUi(this);
}

TabPage::~TabPage()
{
    delete ui;
}

void TabPage::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
