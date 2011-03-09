#include "addclasswizard.h"
#include "ui_addclasswizard.h"
#include "inputclassinfowizardpage.h"

AddClassWizard::AddClassWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddClassWizard)
{
    ui->setupUi(this);

    setFixedSize(width(), height());
    setButtonText(QWizard::FinishButton, tr("完成"));
    setButtonText(QWizard::CancelButton, tr("取消"));

    addPage(new InputClassInfoWizardPage);
    qDebug() << "AddClassWizard:" << this->size();
}

AddClassWizard::~AddClassWizard()
{
    delete ui;
}
