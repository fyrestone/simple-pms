#include "dataengine.h"
#include <QGlobalStatic>    /* 使用Q_GLOBAL_STATIC来单例化 */

using namespace DataEngine;

Q_GLOBAL_STATIC(Task, task)

Task::Task()
{
    connect(&initializeDBTask,                  SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&loginTask,                         SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&insertOrUpdateClassTask,           SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&fillAccountsListModelTask,         SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&fillNavigationTreeTask,            SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&fillGradeListTask,                 SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
}

Task *Task::instance()
{
    return task();
}

void Task::waitForFinished(int task)
{

}

void Task::initializeDB(const QString &dbPath)
{
    //initializeDBTask.asyncRun(dbPath);
    initializeDBTask.syncRun(dbPath);
}

void Task::login(const QString &id, const QString &pwd, bool save)
{
    loginTask.asyncRun(id, pwd, save);
}

void Task::insertOrUpdateClass(int gradeNum, int classNum, const QString &classType)
{
    insertOrUpdateClassTask.asyncRun(gradeNum, classNum, classType);
}

void Task::fillAccountsListModel(QPointer<QStandardItemModel> model, int max)
{
    fillAccountsListModelTask.syncRun(model, max);
    //fillAccountsListModelTask.asyncRun(model, max);
}

void Task::fillNavigationTree(QPointer<QTreeWidget> widget, const QString &rootName)
{
    fillNavigationTreeTask.asyncRun(widget, rootName);
}

void Task::fillGradeList(QPointer<QTreeWidget> widget, const QString &headName)
{
    fillGradeListTask.asyncRun(widget, headName);
}

void Task::registerTask(Tasks taskID, AbstractBaseTask *taskPtr)
{
    connect(taskPtr,            SIGNAL(finished(int,QVariant)),
            this,               SIGNAL(finished(int,QVariant)));

    taskSet.insert(taskID, taskPtr);
}
