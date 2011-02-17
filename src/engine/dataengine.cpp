#include "dataengine.h"
#include <QGlobalStatic>    /* 使用Q_GLOBAL_STATIC来单例化 */

using namespace DataEngine;

Q_GLOBAL_STATIC(Task, task)

Task::Task()
{
    connect(&initializeDBTask, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SIGNAL(finished(DataEngine::Tasks,QVariant)));
    connect(&loginTask, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SIGNAL(finished(DataEngine::Tasks,QVariant)));
    connect(&fillAccountsListModelTask, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SIGNAL(finished(DataEngine::Tasks,QVariant)));
}

Task *Task::instance()
{
    return task();
}

void Task::initializeDB(const QString &dbPath)
{
    initializeDBTask.run(dbPath);
}

void Task::login(const QString &id, const QString &pwd)
{
    loginTask.run(id, pwd);
}

void Task::fillAccountsListModel(QStandardItemModel *model, int max)
{
    fillAccountsListModelTask.run(model, max);
}