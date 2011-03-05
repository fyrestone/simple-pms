#include "addgradewizard.h"
#include "ui_addgradewizard.h"
#include "inputgradeinfowizardpage.h"

AddGradeWizard::AddGradeWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddGradeWizard)
{
    ui->setupUi(this);
    addPage(new InputGradeInfoWizardPage);
}

AddGradeWizard::~AddGradeWizard()
{
    delete ui;
}
