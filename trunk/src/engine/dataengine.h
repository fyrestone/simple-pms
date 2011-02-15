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

        void createTable();

    signals:
        void finished(DataEngine::Tasks name, const QVariant &result);

    private:
        CreateTableTask m_createTable;
    };
}

#endif // DATAENGINE_H
