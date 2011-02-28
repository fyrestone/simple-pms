#include "classmgmtdlg.h"
#include "classmgmtdlg_p.h"
#include "ui_classmgmtdlg.h"

ClassMgmtDlgPrivate::ClassMgmtDlgPrivate(ClassMgmtDlg *parent)
    :task(DataEngine::Task::instance()),
    q(parent)
{

}

void ClassMgmtDlgPrivate::finished(int taskID, const QVariant &result)
{

}

ClassMgmtDlg::ClassMgmtDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassMgmtDlg),
    d(new ClassMgmtDlgPrivate(this))
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
