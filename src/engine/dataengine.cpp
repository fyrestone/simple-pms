#include "dataengine.h"
#include <QGlobalStatic>    /* 使用Q_GLOBAL_STATIC来单例化 */

using namespace DataEngine;

Q_GLOBAL_STATIC(Task, task)

Task::Task()
{
    connect(&m_createTable, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SIGNAL(finished(DataEngine::Tasks,QVariant)));
}

Task *Task::instance()
{
    return task();
}

void Task::createTable()
{
    m_createTable.run();
}
