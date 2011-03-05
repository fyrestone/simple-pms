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
    connect(&fillAccountsListModelTask,         SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&fillNavigationTreeTask,            SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&insertOrUpdateNavigationTreeTask,  SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
    connect(&fillGradeListTask,                 SIGNAL(finished(int,QVariant)),
            this,                               SIGNAL(finished(int,QVariant)));
}

Task *Task::instance()
{
    return task();
}
