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
    AbstractTaskBase *thisTask = taskSet.value(task);

    if(thisTask) thisTask->waitForFinished();
}

void Task::registerTask(Tasks taskID, AbstractTaskBase *taskPtr)
{
    connect(taskPtr,            SIGNAL(finished(int,QVariant)),
            this,               SIGNAL(finished(int,QVariant)));

    taskSet.insert(taskID, taskPtr);
}
