#include "selectgradewizardpage.h"
#include "selectgradewizardpage_p.h"
#include "ui_selectgradewizardpage.h"

SelectGradeWizardPagePrivate::SelectGradeWizardPagePrivate(SelectGradeWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{

}

inline void SelectGradeWizardPagePrivate::initializeMember()
{

}

inline void SelectGradeWizardPagePrivate::connectSignalsAndSlots()
{

}

inline void SelectGradeWizardPagePrivate::completeConstruct()
{

}

inline bool SelectGradeWizardPagePrivate::validatePage()
{
    return true;
}

SelectGradeWizardPage::SelectGradeWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectGradeWizardPage),
    d(new SelectGradeWizardPagePrivate(this))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
}

SelectGradeWizardPage::~SelectGradeWizardPage()
{
    delete ui;
    delete d;
}

void SelectGradeWizardPage::initializePage()
{
    d->completeConstruct();
}

bool SelectGradeWizardPage::isComplete() const
{
    return true;
}

bool SelectGradeWizardPage::validatePage()
{
    return d->validatePage();
}
