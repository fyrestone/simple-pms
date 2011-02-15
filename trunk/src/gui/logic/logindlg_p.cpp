#include "logindlg_p.h"

LoginDlgPrivate::LoginDlgPrivate(LoginDlg *parent)
    : q_ptr(parent)
{
    DataEngine::Task *task = DataEngine::Task::instance();
    connect(task, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SLOT(finished(DataEngine::Tasks,QVariant)));
    task->createTable();
}

void LoginDlgPrivate::finished(DataEngine::Tasks name, const QVariant &result)
{
    qDebug() << "finished with:" << name << result;
}
