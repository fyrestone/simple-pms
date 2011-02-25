#include "classmgmtdlg.h"
#include "ui_classmgmtdlg.h"

ClassMgmtDlg::ClassMgmtDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassMgmtDlg)
{
    ui->setupUi(this);
}

ClassMgmtDlg::~ClassMgmtDlg()
{
    delete ui;
}

void ClassMgmtDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
