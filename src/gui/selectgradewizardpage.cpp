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
    q->ui->lineEdit->setVisible(false);                             //不可见的lineEdit，用于绑定field
    q->registerField("gradeNum", q->ui->lineEdit);                  //注册gradeNum域
}

inline void SelectGradeWizardPagePrivate::connectSignalsAndSlots()
{
    connect(q->ui->gradeTreeWidget,     SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,                       SLOT(doItemClicked(QTreeWidgetItem*,int)));
}

inline void SelectGradeWizardPagePrivate::completeConstruct()
{
    QPointer<QTreeWidget> gradeTreePtr = q->ui->gradeTreeWidget;    //年级列表控件指针

    (void)task->lookup<DataEngine::FillGradeListTask>()->run(gradeTreePtr, tr("当前所有年级"));
}

inline bool SelectGradeWizardPagePrivate::validatePage()
{
    QList<QTreeWidgetItem *> selectedSet = q->ui->gradeTreeWidget->selectedItems();

    return !selectedSet.isEmpty();
}

void SelectGradeWizardPagePrivate::doItemClicked(QTreeWidgetItem *item, int column)
{
    q->setField("gradeNum", item->data(column, Qt::DisplayRole));
    emit q->completeChanged();
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
    return d->validatePage();
}

bool SelectGradeWizardPage::validatePage()
{
    return d->validatePage();
}
