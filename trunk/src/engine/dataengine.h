#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "abstracttask.h"

namespace DataEngine
{
    class Task : public QObject
    {
        Q_OBJECT

    public:
        Task();
        static Task *instance();

        inline void initializeDB(const QString &dbPath);
        inline void login(const QString &id, const QString &pwd, bool save);
        inline void fillAccountsListModel(QStandardItemModel *model, int max = 10);
        inline void fillNavigationTree(QTreeWidget *widget, const QString &rootName);
        inline void insertOrUpdateNavigationTree(QTreeWidget *widget,
                                                 int gradeNum,
                                                 int classNum,
                                                 const QString &classType);

    signals:
        void finished(int task, const QVariant &result);

    private:
        Q_DISABLE_COPY(Task)
        InitializeDBTask                    initializeDBTask;
        LoginTask                           loginTask;
        FillAccountsListModelTask           fillAccountsListModelTask;
        FillNavigationTreeTask              fillNavigationTreeTask;
        InsertOrUpdateNavigationTreeTask    insertOrUpdateNavigationTreeTask;
    };

    inline void Task::initializeDB(const QString &dbPath)
    {
        initializeDBTask.run(dbPath);
    }

    inline void Task::login(const QString &id, const QString &pwd, bool save)
    {
        loginTask.run(id, pwd, save);
    }

    inline void Task::fillAccountsListModel(QStandardItemModel *model, int max)
    {
        fillAccountsListModelTask.run(model, max);
    }

    inline void Task::fillNavigationTree(QTreeWidget *widget, const QString &rootName)
    {
        fillNavigationTreeTask.run(widget, rootName);
    }

    inline void Task::insertOrUpdateNavigationTree(QTreeWidget *widget,
                                                   int gradeNum,
                                                   int classNum,
                                                   const QString &classType)
    {
        insertOrUpdateNavigationTreeTask.run(widget, gradeNum, classNum, classType);
    }
}

#endif // DATAENGINE_H
