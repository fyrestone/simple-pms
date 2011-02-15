#include "abstracttask.h"

using namespace DataEngine;

/* 元类型声明 */
int i = qRegisterMetaType<Tasks>("DataEngine::Tasks");

void CreateTableTask::run()
{
    watchFuture(QtConcurrent::run(this, &CreateTableTask::createTable));
}

bool CreateTableTask::createTable()
{
    qDebug() << "create table";
    return true;
}
