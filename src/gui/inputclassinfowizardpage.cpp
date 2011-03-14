#include "inputclassinfowizardpage.h"
#include "inputclassinfowizardpage_p.h"
#include "ui_inputclassinfowizardpage.h"

#include <QIntValidator>

InputClassInfoWizardPagePrivate::InputClassInfoWizardPagePrivate(InputClassInfoWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent),
    hasTaskRunning(true)
{
}

inline void InputClassInfoWizardPagePrivate::initializeMember()
{
    q->ui->classLineEdit->setValidator(new QIntValidator(this));
    q->registerField("classNum", q->ui->classLineEdit);
}

inline void InputClassInfoWizardPagePrivate::connectSignalsAndSlots()
{
    connect(task,                       SIGNAL(finished(int,QVariant)),
            this,                       SLOT(finished(int,QVariant)));
    connect(q->ui->classLineEdit,       SIGNAL(textChanged(QString)),
            q,                          SIGNAL(completeChanged()));
}

inline void InputClassInfoWizardPagePrivate::completeConstruct()
{
    hasTaskRunning = true;

    QPointer<QTreeWidget> classTreePtr = q->ui->classTreeWidget;
    task->lookup<DataEngine::FillClassListTask>()->run(classTreePtr, tr("已经存在的班级"), q->field("gradeNum").toInt());
}

inline bool InputClassInfoWizardPagePrivate::isComplete() const
{
    if(hasTaskRunning)
        return false;
    else
    {
        QString classNumStr = q->ui->classLineEdit->text();
        QList<QTreeWidgetItem *> match = q->ui->classTreeWidget->findItems(classNumStr, Qt::MatchExactly);

        return !classNumStr.isEmpty() && match.isEmpty();
    }
}

inline bool InputClassInfoWizardPagePrivate::validatePage()
{
    bool success = false;

    int gradeNum = q->field("gradeNum").toInt(&success);

    if(success)
    {
        using namespace DataEngine;

        task->lookup<InsertOrUpdateClassTask>();
        //DataEngine::InsertOrUpdateClassTask *fucktask = task->lookup<DataEngine::InsertOrUpdateClassTask>(DataEngine::InsertOrUpdateClass);
        //if(fucktask) fucktask->run(gradeNum, q->ui->classLineEdit->text().toInt(), tr("普通班"));
        //task->insertOrUpdateClass(gradeNum, q->ui->classLineEdit->text().toInt(), tr("普通班"));
        //success = task->waitForResult(DataEngine::InsertOrUpdateClass).toBool();
    }

    return success;
}

void InputClassInfoWizardPagePrivate::finished(int taskID, const QVariant &result)
{
    switch(taskID)
    {
    case DataEngine::FillClassList:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            hasTaskRunning = false;
        }
        break;
    }
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
    return d->isComplete();
}

bool InputClassInfoWizardPage::validatePage()
{
    return d->validatePage();
}

