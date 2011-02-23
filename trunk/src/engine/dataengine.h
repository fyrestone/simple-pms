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
        void fillClassTreeWidget(QTreeWidget *widget);
        void insertGradeClass(int gradeNum, int classNum, int classType = 0);

    signals:
        void finished(DataEngine::Tasks name, const QVariant &result);

    private:
        Q_DISABLE_COPY(Task)
        InitializeDBTask initializeDBTask;
        LoginTask loginTask;
        FillAccountsListModelTask fillAccountsListModelTask;
        FillClassTreeWidgetTask fillClassTreeWidgetTask;
        InsertGradeClassTask insertGradeClassTask;
    };
}

#endif // DATAENGINE_H
