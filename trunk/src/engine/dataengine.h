#ifndef DATAENGINE_H
#define DATAENGINE_H

#include "abstracttask.h"

class QStandardItemModel;

namespace DataEngine
{
    class Task : public QObject
    {
        Q_OBJECT

    public:
        Task();
        static Task *instance();

        void initializeDB(const QString &dbPath);
        void login(const QString &id, const QString &pwd);
        void fillAccountsListModel(QStandardItemModel *model, int max = 10);

    signals:
        void finished(DataEngine::Tasks name, const QVariant &result);

    private:
        InitializeDBTask initializeDBTask;
        LoginTask loginTask;
        FillAccountsListModelTask fillAccountsListModelTask;
    };
}

#endif // DATAENGINE_H
