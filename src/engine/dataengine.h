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

    private:
        void registerTask(Tasks taskID, AbstractBaseTask *taskPtr);

        template<typename TaskType>
        inline TaskType *lookupTask(Tasks taskID) const
        { return static_cast<TaskType *>(taskSet.value(taskID, NULL)); }

    private:
        Q_DISABLE_COPY(Task)
        QHash<Tasks, AbstractBaseTask *> taskSet;
    };
}

#endif // DATAENGINE_H
