#include "inputgradeinfowizardpage.h"
#include "inputgradeinfowizardpage_p.h"
#include "ui_inputgradeinfowizardpage.h"

#include <QIntValidator>

InputGradeInfoWizardPagePrivate::InputGradeInfoWizardPagePrivate(InputGradeInfoWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{
}

void InputGradeInfoWizardPagePrivate::initializeMember()
{
    q->ui->gradeLineEdit->setValidator(new QIntValidator(this));
}

void InputGradeInfoWizardPagePrivate::connectSignalsAndSlots()
{

}

void InputGradeInfoWizardPagePrivate::completeConstruct()
{
    task->fillGradeList(q->ui->gradeTreeWidget, tr("已经存在的年级"));
}

void InputGradeInfoWizardPagePrivate::finished(int taskID, const QVariant &result)
{

}


InputGradeInfoWizardPage::InputGradeInfoWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::InputGradeInfoWizardPage),
    d(new InputGradeInfoWizardPagePrivate(this))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
}

InputGradeInfoWizardPage::~InputGradeInfoWizardPage()
{
    delete ui;
    delete d;
}

void InputGradeInfoWizardPage::initializePage()
{
    d->completeConstruct();
}
