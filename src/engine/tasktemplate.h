#ifndef TASKTEMPLATE_H
#define TASKTEMPLATE_H

namespace DataEngine
{
class AbstractBaseTask : public QObject
{
    Q_OBJECT

public:
    AbstractBaseTask(QObject *parent = 0) : QObject(parent) {}
    ~AbstractBaseTask() {}

signals:
    /* 此处task不用枚举为了避免产生“enumeration value '***' not handled in switch”警告 */
    void finished(int task, const QVariant &result);

private slots:
    virtual void finishDispatcher() = 0;
};

template<int N, typename T>
class AbstractTask : public AbstractBaseTask
{
public:
    AbstractTask(QObject *parent = 0);
    ~AbstractTask();

protected:
    template<typename Class>
    void asyncRun(T (Class::*runFn)());
    template<typename Class, typename Param1, typename Arg1>
    void asyncRun(T (Class::*runFn)(Param1), const Arg1 &arg1);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
    void asyncRun(T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
    void asyncRun(T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
    void asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
    void asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5);

    template<typename Class>
    void asyncRun(bool (Class::*initFn)(), T (Class::*runFn)());
    template<typename Class, typename Param1, typename Arg1>
    void asyncRun(bool (Class::*initFn)(Param1), T (Class::*runFn)(Param1), const Arg1 &arg1);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
    void asyncRun(bool (Class::*initFn)(Param1, Param2), T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
    void asyncRun(bool (Class::*initFn)(Param1, Param2, Param3), T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
    void asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4), T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4);
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
    void asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4, Param5), T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5);

private:
    void finishDispatcher();

private:
    QFutureWatcher<T> watcher;
    static QFutureSynchronizer<void> synchronizer;
};

template<int N, typename T>
AbstractTask<N, T>::AbstractTask(QObject *parent)
    :AbstractBaseTask(parent)
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(finishDispatcher()));
}

template<int N, typename T>
AbstractTask<N, T>::~AbstractTask()
{

}

template<int N, typename T>
template<typename Class>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)())
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1), const Arg1 &arg1)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
inline void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4, arg5));
}

template<int N, typename T>
template<typename Class>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(), T (Class::*runFn)())
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)())
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1), T (Class::*runFn)(Param1), const Arg1 &arg1)
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)(arg1))
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2), T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)(arg1, arg2))
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3), T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3))
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4), T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3, arg4))
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4));
}

template<int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
inline void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4, Param5), T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    watcher.waitForFinished();
    if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3, arg4, arg5))
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4, arg5));
}

template<int N, typename T>
void AbstractTask<N, T>::finishDispatcher()
{
    emit finished(N, QVariant(watcher.result()));
}
}

#endif // TASKTEMPLATE_H
