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

    FuncType funcPtr() const {return ptr;}

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

    void asyncRun();
    template<typename Arg1>
    void asyncRun(const Arg1 &arg1);
    template<typename Arg1, typename Arg2>
    void asyncRun(const Arg1 &arg1, const Arg2 &arg2);
    template<typename Arg1, typename Arg2, typename Arg3>
    void asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    void asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    void asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5);

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
inline void AbstractTask<C, N, T>::asyncRun()
{
    typedef const MemberFuncWrapper<T (C::*)()> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr()));
}

template<typename C, int N, typename T>
template<typename Arg1>
inline void AbstractTask<C, N, T>::asyncRun(const Arg1 &arg1)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1));
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2>
inline void AbstractTask<C, N, T>::asyncRun(const Arg1 &arg1, const Arg2 &arg2)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2));
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3>
inline void AbstractTask<C, N, T>::asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3));
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline void AbstractTask<C, N, T>::asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4));
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline void AbstractTask<C, N, T>::asyncRun(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4, Arg5)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper) watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4, arg5));
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