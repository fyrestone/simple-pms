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

        void initializeDB(const QString &dbPath);
        void login(const QString &id, const QString &pwd, bool save);
        void fillAccountsListModel(QStandardItemModel *model, int max = 10);
        void fillNavigationTree(QTreeWidget *widget, const QString &rootName);
        void insertOrUpdateNavigationTree(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType);

    signals:
        void finished(DataEngine::Tasks name, const QVariant &result);

    private:
        Q_DISABLE_COPY(Task)
        InitializeDBTask                    initializeDBTask;
        LoginTask                           loginTask;
        FillAccountsListModelTask           fillAccountsListModelTask;
        FillNavigationTreeTask              fillNavigationTreeTask;
        InsertOrUpdateNavigationTreeTask    insertOrUpdateNavigationTreeTask;
        //InsertOrUpdateClassTreeTask insertOrUpdateClassTreeTask;
    };
}

#endif // DATAENGINE_H
