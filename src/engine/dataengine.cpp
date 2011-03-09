#include "dataengine.h"
#include <QGlobalStatic>    /* 使用Q_GLOBAL_STATIC来单例化 */

using namespace DataEngine;

Q_GLOBAL_STATIC(Task, task)

Task::Task()
{
    registerTask(InitializeDB,          new InitializeDBTask(this));
    registerTask(Login,                 new LoginTask(this));
    registerTask(InsertOrUpdateClass,   new InsertOrUpdateClassTask(this));
    registerTask(FillAccountsListModel, new FillAccountsListModelTask(this));
    registerTask(FillNavigationTree,    new FillNavigationTreeTask(this));
    registerTask(FillGradeList,         new FillGradeListTask(this));
    registerTask(FillClassList,         new FillClassListTask(this));
}

Task *Task::instance()
{
    return task();
}

void Task::waitForFinished(Tasks task)
{
    AbstractBaseTask *thisTask = taskSet.value(task);

    if(thisTask) thisTask->waitForFinished();
}

void Task::initializeDB(const QString &dbPath)
{
    InitializeDBTask *thisTask = lookupTask<InitializeDBTask>(InitializeDB);

    if(thisTask) thisTask->syncRun(dbPath);
}

void Task::login(const QString &id, const QString &pwd, bool save)
{
    LoginTask *thisTask = lookupTask<LoginTask>(Login);

    if(thisTask) thisTask->asyncRun(id, pwd, save);
}

void Task::insertOrUpdateClass(int gradeNum, int classNum, const QString &classType)
{
    InsertOrUpdateClassTask *thisTask = lookupTask<InsertOrUpdateClassTask>(InsertOrUpdateClass);

    if(thisTask) thisTask->asyncRun(gradeNum, classNum, classType);
}

void Task::fillAccountsListModel(QPointer<QStandardItemModel> model, int max)
{
    FillAccountsListModelTask *thisTask = lookupTask<FillAccountsListModelTask>(FillAccountsListModel);

    if(thisTask) thisTask->syncRun(model, max);
}

void Task::fillNavigationTree(QPointer<QTreeWidget> widget, const QString &rootName)
{
    FillNavigationTreeTask *thisTask = lookupTask<FillNavigationTreeTask>(FillNavigationTree);

    if(thisTask) thisTask->asyncRun(widget, rootName);
}

void Task::fillGradeList(QPointer<QTreeWidget> widget, const QString &headName)
{
    FillGradeListTask *thisTask = lookupTask<FillGradeListTask>(FillGradeList);

    if(thisTask) thisTask->asyncRun(widget, headName);
}

void Task::fillClassList(QPointer<QTreeWidget> widget, const QString &headName, int gradeNum)
{
    FillClassListTask *thisTask = lookupTask<FillClassListTask>(FillClassList);

    if(thisTask) thisTask->asyncRun(widget, headName, gradeNum);
}

void Task::registerTask(Tasks taskID, AbstractBaseTask *taskPtr)
{
    connect(taskPtr,            SIGNAL(finished(int,QVariant)),
            this,               SIGNAL(finished(int,QVariant)));

    taskSet.insert(taskID, taskPtr);
}
