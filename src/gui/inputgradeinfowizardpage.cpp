#include "inputgradeinfowizardpage.h"
#include "inputgradeinfowizardpage_p.h"
#include "ui_inputgradeinfowizardpage.h"

#include <QIntValidator>

InputGradeInfoWizardPagePrivate::InputGradeInfoWizardPagePrivate(InputGradeInfoWizardPage *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{
}

inline void InputGradeInfoWizardPagePrivate::initializeMember()
{
    q->ui->gradeLineEdit->setValidator(new QIntValidator(this));    //设置整数验证器
    q->registerField("gradeNum", q->ui->gradeLineEdit);             //注册gradeNum域
}

inline void InputGradeInfoWizardPagePrivate::connectSignalsAndSlots()
{
    connect(q->ui->gradeLineEdit,       SIGNAL(textChanged(QString)),
            q,                          SIGNAL(completeChanged()));
}

inline void InputGradeInfoWizardPagePrivate::completeConstruct()
{
    QPointer<QTreeWidget> gradeTreePtr = q->ui->gradeTreeWidget;    //年级列表控件指针

    (void)task->lookup<DataEngine::FillGradeListTask>()->run(gradeTreePtr, tr("已经存在的年级"));
}

inline bool InputGradeInfoWizardPagePrivate::validatePage()
{
    int gradeNum = q->ui->gradeLineEdit->text().toInt();            //年级编号

    return task->lookup<DataEngine::InsertOrUpdateClassTask>()->run(gradeNum, 1, tr("普通班"));
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

bool InputGradeInfoWizardPage::isComplete() const
{
    QString gradeNumStr = field("gradeNum").toString();
    QList<QTreeWidgetItem *> match = ui->gradeTreeWidget->findItems(gradeNumStr, Qt::MatchExactly);

    return !gradeNumStr.isEmpty() && match.isEmpty();
}

bool InputGradeInfoWizardPage::validatePage()
{
    return d->validatePage();
}
