#include "inputclassinfowizardpage.h"
#include "inputclassinfowizardpage_p.h"
#include "ui_inputclassinfowizardpage.h"

#include <QIntValidator>

InputClassInfoWizardPagePrivate::InputClassInfoWizardPagePrivate(InputClassInfoWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{
}

inline void InputClassInfoWizardPagePrivate::initializeMember()
{
    q->ui->classLineEdit->setValidator(new QIntValidator(this));    //设置整数验证器
    q->registerField("classNum", q->ui->classLineEdit);             //注册classNum域
}

inline void InputClassInfoWizardPagePrivate::connectSignalsAndSlots()
{
    connect(q->ui->classLineEdit,       SIGNAL(textChanged(QString)),
            q,                          SIGNAL(completeChanged()));
}

inline void InputClassInfoWizardPagePrivate::completeConstruct()
{
    QPointer<QTreeWidget> classTreePtr = q->ui->classTreeWidget;    //班级列表控件指针
    int gradeNum = q->field("gradeNum").toInt();                    //年级编号

    (void)task->lookup<DataEngine::FillClassListTask>()->run(classTreePtr, tr("已经存在的班级"), gradeNum);
}

inline bool InputClassInfoWizardPagePrivate::validatePage()
{
    bool success = false;

    int gradeNum = q->field("gradeNum").toInt(&success);            //年级编号
    int classNum = q->ui->classLineEdit->text().toInt();            //班级编号

    return success && task->lookup<DataEngine::InsertOrUpdateClassTask>()->run(gradeNum, classNum, tr("普通班"));
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
    QString classNumStr = ui->classLineEdit->text();
    QList<QTreeWidgetItem *> match = ui->classTreeWidget->findItems(classNumStr, Qt::MatchExactly);

    return !classNumStr.isEmpty() && match.isEmpty();
}

bool InputClassInfoWizardPage::validatePage()
{
    return d->validatePage();
}

