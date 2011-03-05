#include "addclasswizard.h"
#include "ui_addclasswizard.h"

AddClassWizard::AddClassWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AddClassWizard)
{
    ui->setupUi(this);
}

AddClassWizard::~AddClassWizard()
{
    delete ui;
}
