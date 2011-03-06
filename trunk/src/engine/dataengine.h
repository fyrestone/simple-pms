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

        void waitForFinished(int task);

    signals:
        void finished(int task, const QVariant &result);

    public slots:
        void initializeDB(const QString &dbPath);
        void login(const QString &id, const QString &pwd, bool save);
        void insertOrUpdateClass(int gradeNum, int classNum, const QString &classType);
        void fillAccountsListModel(QStandardItemModel *model, int max = 10);
        void fillNavigationTree(QTreeWidget *widget, const QString &rootName);
        void fillGradeList(QTreeWidget *widget, const QString &headName);

    private:
        Q_DISABLE_COPY(Task)
        InitializeDBTask                    initializeDBTask;
        LoginTask                           loginTask;
        InsertOrUpdateClassTask             insertOrUpdateClassTask;
        FillAccountsListModelTask           fillAccountsListModelTask;
        FillNavigationTreeTask              fillNavigationTreeTask;
        FillGradeListTask                   fillGradeListTask;
    };

    inline void Task::waitForFinished(int task)
    {

    }

    inline void Task::initializeDB(const QString &dbPath)
    {
        initializeDBTask.run(dbPath);
    }

    inline void Task::login(const QString &id, const QString &pwd, bool save)
    {
        loginTask.run(id, pwd, save);
    }

    inline void Task::insertOrUpdateClass(int gradeNum, int classNum, const QString &classType)
    {
        insertOrUpdateClassTask.run(gradeNum, classNum, classType);
    }

    inline void Task::fillAccountsListModel(QStandardItemModel *model, int max)
    {
        fillAccountsListModelTask.run(model, max);
    }

    inline void Task::fillNavigationTree(QTreeWidget *widget, const QString &rootName)
    {
        fillNavigationTreeTask.run(widget, rootName);
    }

    inline void Task::fillGradeList(QTreeWidget *widget, const QString &headName)
    {
        fillGradeListTask.run(widget, headName);
    }
}

#endif // DATAENGINE_H
