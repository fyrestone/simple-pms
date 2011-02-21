#include "tabpage.h"
#include "tabpage_p.h"
#include "ui_tabpage.h"

TabPage::TabPage(QWidget *parent) :
    QFrame(parent),
    d_ptr(new TabPagePrivate(this)),
    ui(new Ui::TabPage)
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
