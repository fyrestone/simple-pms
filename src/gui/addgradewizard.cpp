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
    setButtonText(QWizard::FinishButton, tr("完成"));
    setButtonText(QWizard::CancelButton, tr("取消"));

    addPage(new InputGradeInfoWizardPage);
    qDebug() << "AddGradeWizard:" << this->size();
}

AddGradeWizard::~AddGradeWizard()
{
    delete ui;
}
