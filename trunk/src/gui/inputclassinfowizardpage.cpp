#include "inputclassinfowizardpage.h"
#include "inputclassinfowizardpage_p.h"
#include "ui_inputclassinfowizardpage.h"

#include <QIntValidator>

InputClassInfoWizardPagePrivate::InputClassInfoWizardPagePrivate(InputClassInfoWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{
}

void InputClassInfoWizardPagePrivate::initializeMember()
{
    q->ui->classLineEdit->setValidator(new QIntValidator(this));
    q->registerField("classNum", q->ui->classLineEdit);
}

void InputClassInfoWizardPagePrivate::connectSignalsAndSlots()
{
    connect(q->ui->classLineEdit,       SIGNAL(textChanged(QString)),
            q,                          SIGNAL(completeChanged()));
}

void InputClassInfoWizardPagePrivate::completeConstruct()
{
    task->fillClassList(q->ui->classTreeWidget, tr("已经存在的班级"), 2010);
}

void InputClassInfoWizardPagePrivate::finished(int taskID, const QVariant &result)
{

}

InputClassInfoWizardPage::InputClassInfoWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::InputClassInfoWizardPage),
    d(new InputClassInfoWizardPagePrivate(this))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
}

InputClassInfoWizardPage::~InputClassInfoWizardPage()
{
    delete ui;
    delete d;
}

void InputClassInfoWizardPage::initializePage()
{
    d->completeConstruct();
}

bool InputClassInfoWizardPage::isComplete() const
{
    QString classNum = field("classNum").toString();
    QList<QTreeWidgetItem *> match = ui->classTreeWidget->findItems(classNum, Qt::MatchExactly);

    return !classNum.isEmpty() && match.isEmpty();
}

