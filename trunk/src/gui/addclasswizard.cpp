#include "addclasswizard.h"
#include "ui_addclasswizard.h"
#include "selectgradewizardpage.h"
#include "inputclassinfowizardpage.h"

AddClassWizard::AddClassWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddClassWizard)
{
    ui->setupUi(this);

    setFixedSize(width(), height());

    setButtonText(QWizard::BackButton, tr("上一步"));
    setButtonText(QWizard::NextButton, tr("下一步"));
    setButtonText(QWizard::CommitButton, tr("提交"));
    setButtonText(QWizard::FinishButton, tr("完成"));
    setButtonText(QWizard::CancelButton, tr("取消"));
    setButtonText(QWizard::HelpButton, tr("帮助"));

    addPage(new SelectGradeWizardPage);
    addPage(new InputClassInfoWizardPage);
}

AddClassWizard::~AddClassWizard()
{
    delete ui;
}
