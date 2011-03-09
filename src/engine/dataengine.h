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
        void fillAccountsListModel(QPointer<QStandardItemModel> model, int max = 10);
        void fillNavigationTree(QPointer<QTreeWidget> widget, const QString &rootName);
        void fillGradeList(QPointer<QTreeWidget> widget, const QString &headName);

    private:
        Q_DISABLE_COPY(Task)
        InitializeDBTask                    initializeDBTask;
        LoginTask                           loginTask;
        InsertOrUpdateClassTask             insertOrUpdateClassTask;
        FillAccountsListModelTask           fillAccountsListModelTask;
        FillNavigationTreeTask              fillNavigationTreeTask;
        FillGradeListTask                   fillGradeListTask;
    };
}

#endif // DATAENGINE_H
