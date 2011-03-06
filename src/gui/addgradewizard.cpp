#include "addgradewizard.h"
#include "ui_addgradewizard.h"
#include "inputgradeinfowizardpage.h"

AddGradeWizard::AddGradeWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddGradeWizard)
{
    ui->setupUi(this);

    setFixedSize(width(), height());
    setButtonText(QWizard::FinishButton, tr("完成"));
    setButtonText(QWizard::CancelButton, tr("取消"));

    addPage(new InputGradeInfoWizardPage);
}

AddGradeWizard::~AddGradeWizard()
{
    delete ui;
}
