#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "tasks.h"

namespace DataEngine
{
//class TaskBase : public QObject
//{
//    Q_OBJECT

//public:
//    TaskBase(QObject *parent = 0) : QObject(parent) {}
//    ~TaskBase() {}

//signals:
//    /* 此处task不用枚举为了避免产生“enumeration value '***' not handled in switch”警告 */
//    void finished(int task, const QVariant &result);
//};

#define FUCK(x) x##Task

class Task : public QObject
{
    Q_OBJECT

public:
    Task();
    static Task *instance();

    void waitForFinished(Tasks task);

    template<typename TaskType>
    inline TaskType *lookup() const
    {
        TaskType * ff = static_cast<TaskType *>(taskSet.value(TaskType::type, NULL));
        return static_cast<TaskType *>(taskSet.value(TaskType::type, NULL));
    }

signals:
    /* 此处task不用枚举为了避免产生“enumeration value '***' not handled in switch”警告 */
    void finished(int task, const QVariant &result);

public slots:
    void initializeDB(const QString &dbPath);
    void login(const QString &id, const QString &pwd, bool save);
    void insertOrUpdateClass(int gradeNum, int classNum, const QString &classType);
    void fillAccountsListModel(QPointer<QStandardItemModel> model, int max = 10);
    void fillNavigationTree(QPointer<QTreeWidget> widget, const QString &rootName);
    void fillGradeList(QPointer<QTreeWidget> widget, const QString &headName);
    void fillClassList(QPointer<QTreeWidget> widget, const QString &headName, int gradeNum);

private:
    void registerTask(Tasks taskID, AbstractTaskBase *taskPtr);

private:
    Q_DISABLE_COPY(Task)
    QHash<int, AbstractTaskBase *> taskSet;
};
}

#endif // DATAENGINE_H
