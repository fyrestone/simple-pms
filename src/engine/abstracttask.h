#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include <QObject>
#include <QtConcurrentRun>
#include <QFutureWatcher>

namespace DataEngine
{
    enum Tasks              ///  任务枚举
    {
        CreateTable = 3     ///< 创建表
    };

    class AbstractBaseTask : public QObject
    {
        Q_OBJECT

    public:
        AbstractBaseTask(QObject *parent = 0) : QObject(parent) {}
        ~AbstractBaseTask() {}

    signals:
        void finished(DataEngine::Tasks name, const QVariant &result);

    private slots:
        virtual void finishDispatcher() = 0;
    };

    template<Tasks N, typename T>
    class AbstractTask : public AbstractBaseTask
    {
    public:
        AbstractTask(QObject *parent = 0);
        ~AbstractTask();

    protected:
        void watchFuture(const QFuture<T> &future);

    private:
        void finishDispatcher();

    private:
        static const Tasks name = N;
        QFutureWatcher<T> watcher;
    };

    template<Tasks N, typename T>
    AbstractTask<N, T>::AbstractTask(QObject *parent)
        :AbstractBaseTask(parent)
    {
        connect(&watcher, SIGNAL(finished()), this, SLOT(finishDispatcher()));
    }

    template<Tasks N, typename T>
    AbstractTask<N, T>::~AbstractTask()
    {

    }

    template<Tasks N, typename T>
    void AbstractTask<N, T>::watchFuture(const QFuture<T> &future)
    {
        if(watcher.isRunning())
        {
            watcher.cancel();
            watcher.waitForFinished();
        }

        watcher.setFuture(future);
    }

    template<Tasks N, typename T>
    void AbstractTask<N, T>::finishDispatcher()
    {
        emit finished(name, QVariant(watcher.result()));
    }

    class CreateTableTask : public AbstractTask<CreateTable, bool>
    {
    public:
        void run();

    private:
        bool createTable();
    };
}

#endif // ABSTRACTTASK_H
