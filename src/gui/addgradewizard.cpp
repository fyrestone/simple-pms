#include "addgradewizard.h"
#include "ui_addgradewizard.h"
#include "inputgradeinfowizardpage.h"

#include <QDebug>

AddGradeWizard::AddGradeWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddGradeWizard)
{
    ui->setupUi(this);

    setFixedSize(width(), height());

    setButtonText(QWizard::BackButton, tr("上一步"));
    setButtonText(QWizard::NextButton, tr("下一步"));
    setButtonText(QWizard::CommitButton, tr("提交"));
    setButtonText(QWizard::FinishButton, tr("完成"));
    setButtonText(QWizard::CancelButton, tr("取消"));
    setButtonText(QWizard::HelpButton, tr("帮助"));

    addPage(new InputGradeInfoWizardPage);
}

AddGradeWizard::~AddGradeWizard()
{
    delete ui;
}
