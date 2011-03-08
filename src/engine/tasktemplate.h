#ifndef TASKTEMPLATE_H
#define TASKTEMPLATE_H

namespace DataEngine
{
class WrapperBase
{

};

template<typename FuncType>
class MemberFuncWrapper : public WrapperBase
{
public:
    MemberFuncWrapper(FuncType fnPtr) :
        ptr(fnPtr) {}

    FuncType FuncPtr() const {return ptr;}

private:
    FuncType ptr;
};

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

template<typename C, int N, typename T>
class AbstractTask : public AbstractBaseTask
{
public:
    AbstractTask(QObject *parent = 0);
    ~AbstractTask();

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

protected:
    void setAsyncRunEntry(T (C::*runFn)());
    template<typename Param1>
    void setAsyncRunEntry(T (C::*runFn)(Param1));
    template<typename Param1, typename Param2>
    void setAsyncRunEntry(T (C::*runFn)(Param1, Param2));
    template<typename Param1, typename Param2, typename Param3>
    void setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3));
    template<typename Param1, typename Param2, typename Param3, typename Param4>
    void setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4));
    template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
    void setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4, Param5));

private:
    void finishDispatcher();

private:
    QFutureWatcher<T> watcher;
    WrapperBase const *wrapper;
};

template<typename C, int N, typename T>
AbstractTask<C, N, T>::AbstractTask(QObject *parent)
    :AbstractBaseTask(parent), wrapper(0)
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(finishDispatcher()));
}

template<typename C, int N, typename T>
AbstractTask<C, N, T>::~AbstractTask()
{
    delete wrapper;
}

template<typename C, int N, typename T>
template<typename Class>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)())
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn));
}

template<typename C, int N, typename T>
template<typename Class, typename Param1, typename Arg1>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)(Param1), const Arg1 &arg1)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1));
}

template<typename C, int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2));
}

template<typename C, int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3));
}

template<typename C, int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4));
}

template<typename C, int N, typename T>
template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
inline void AbstractTask<C, N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    watcher.waitForFinished();
    watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4, arg5));
}

template<typename C, int N, typename T>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)())
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)()>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)(Param1))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1)>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)(Param1, Param2))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2)>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3)>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3, Param4)>(runFn);

}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
void AbstractTask<C, N, T>::setAsyncRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4, Param5))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3, Param4, Param5)>(runFn);
}

template<typename C, int N, typename T>
void AbstractTask<C, N, T>::finishDispatcher()
{
    emit finished(N, QVariant(watcher.result()));
}
}

#endif // TASKTEMPLATE_H
