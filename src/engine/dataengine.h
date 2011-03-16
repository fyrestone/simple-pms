#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "tasks.h"

namespace DataEngine
{
class Task : public QObject
{
    Q_OBJECT

public:
    Task();
    static Task *instance();

    void waitForFinished(Tasks task);

    template<typename TaskType>
    inline TaskType *lookup() const;

signals:
    /* 此处task不用枚举为了避免产生“enumeration value '***' not handled in switch”警告 */
    void finished(int task, const QVariant &result);

private:
    void registerTask(Tasks taskID, AbstractTaskBase *taskPtr);

private:
    Q_DISABLE_COPY(Task)
    QHash<int, AbstractTaskBase *> taskSet;
};

template<typename TaskType>
inline TaskType *Task::lookup() const
{
    return static_cast<TaskType *>(taskSet.value(TaskType::type, NULL));
}
}

#endif // DATAENGINE_H
