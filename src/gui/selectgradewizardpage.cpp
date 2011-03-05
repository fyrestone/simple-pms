#include "selectgradewizardpage.h"
#include "ui_selectgradewizardpage.h"

SelectGradeWizardPage::SelectGradeWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectGradeWizardPage)
{
    ui->setupUi(this);
}

SelectGradeWizardPage::~SelectGradeWizardPage()
{
    delete ui;
}
