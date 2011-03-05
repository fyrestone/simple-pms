#include "inputclassinfowizardpage.h"
#include "ui_inputclassinfowizardpage.h"

InputClassInfoWizardPage::InputClassInfoWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::InputClassInfoWizardPage)
{
    ui->setupUi(this);
}

InputClassInfoWizardPage::~InputClassInfoWizardPage()
{
    delete ui;
}
