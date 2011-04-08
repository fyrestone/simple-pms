#include "classmgmtdlg.h"
#include "classmgmtdlg_p.h"
#include "ui_classmgmtdlg.h"
#include "studentmgmtframe.h"

ClassMgmtDlgPrivate::ClassMgmtDlgPrivate(ClassMgmtDlg *parent, int gradeNum, int classNum) :
    task(DataEngine::Task::instance()),
    q(parent),
    gradeNum(gradeNum),
    classNum(classNum)
{

}

void ClassMgmtDlgPrivate::initializeMember()
{
    q->ui->tabWidget->addTab(new StudentMgmtFrame(gradeNum, classNum, q), tr("学生信息"));
}

void ClassMgmtDlgPrivate::connectSignalsAndSlots()
{

}

void ClassMgmtDlgPrivate::completeConstruct()
{

}

ClassMgmtDlg::ClassMgmtDlg(int gradeNum, int classNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassMgmtDlg),
    d(new ClassMgmtDlgPrivate(this, gradeNum, classNum))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
    d->completeConstruct();
}

ClassMgmtDlg::~ClassMgmtDlg()
{
    delete ui;
    delete d;
}
