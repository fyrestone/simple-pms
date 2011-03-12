#ifndef TASKTEMPLATE_H
#define TASKTEMPLATE_H

#include <QDebug>   /* 使用qWarning */

namespace DataEngine
{

#define ENGINE_ASYNC_FUNC_NAME  asyncRun        ///  异步接口函数名
#define ENGINE_SYNC_FUNC_NAME   syncRun         ///  同步接口函数名
#define PRINT_NOT_SET_RUN_ENTRY_WARNING() \
    qWarning("please setRunEntry before call %s", __PRETTY_FUNCTION__)
#define PRINT_PARAMETER_RUN_TIME_ERROR() \
    qCritical("pass error arguments in function: %s", __PRETTY_FUNCTION__)

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

template<typename TypeType>
class TypeChecker
{
public:
    static void registerType()
    { ++counter; }
    static bool hasRegistered()
    { return counter; }

private:
    static int counter;
};

template<typename TypeType>
int TypeChecker<TypeType>::counter = 0;

class AbstractTaskBase : public QObject
{
    Q_OBJECT

public:
    AbstractTaskBase(QObject *parent = 0) : QObject(parent) {}
    ~AbstractTaskBase() {}

    virtual void waitForFinished() = 0;

signals:
    /* 此处task不用枚举为了避免产生“enumeration value '***' not handled in switch”警告 */
    void finished(int task, const QVariant &result);

private slots:
    virtual void finishDispatcher() = 0;
};

template<typename C, int N, typename T>
class AbstractTask : public AbstractTaskBase
{
public:
    AbstractTask(QObject *parent = 0);
    ~AbstractTask();

    void ENGINE_ASYNC_FUNC_NAME();
    template<typename Arg1>
    void ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1);
    template<typename Arg1, typename Arg2>
    void ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2);
    template<typename Arg1, typename Arg2, typename Arg3>
    void ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    void ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    void ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5);

    void ENGINE_SYNC_FUNC_NAME();
    template<typename Arg1>
    void ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1);
    template<typename Arg1, typename Arg2>
    void ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2);
    template<typename Arg1, typename Arg2, typename Arg3>
    void ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    void ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4);
    template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    void ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5);

    static const int type = N;

protected:
    void setRunEntry(T (C::*runFn)());
    template<typename Param1>
    void setRunEntry(T (C::*runFn)(Param1));
    template<typename Param1, typename Param2>
    void setRunEntry(T (C::*runFn)(Param1, Param2));
    template<typename Param1, typename Param2, typename Param3>
    void setRunEntry(T (C::*runFn)(Param1, Param2, Param3));
    template<typename Param1, typename Param2, typename Param3, typename Param4>
    void setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4));
    template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
    void setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4, Param5));

    void waitForFinished();

private:
    void finishDispatcher();

private:
    QFutureWatcher<T> watcher;
    WrapperBase const *wrapper;
};

template<typename C, int N, typename T>
AbstractTask<C, N, T>::AbstractTask(QObject *parent)
    :AbstractTaskBase(parent), wrapper(0)
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(finishDispatcher()));
}

template<typename C, int N, typename T>
AbstractTask<C, N, T>::~AbstractTask()
{
    delete wrapper;
}

template<typename C, int N, typename T>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME()
{
    typedef const MemberFuncWrapper<T (C::*)()> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr()));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2)
{
    if(TypeChecker<T (C::*)(Arg1, Arg2)>::hasRegistered())
    {
        typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2)> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4, Arg5)> *WrapperType;

    watcher.waitForFinished();
    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4, arg5));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME()
{
    typedef const MemberFuncWrapper<T (C::*)()> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))());
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1)> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2)> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3)> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4)> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3, arg4));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    typedef const MemberFuncWrapper<T (C::*)(Arg1, Arg2, Arg3, Arg4, Arg5)> *WrapperType;

    WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
    if(fnWrapper)
        emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3, arg4, arg5));
    else
        PRINT_NOT_SET_RUN_ENTRY_WARNING();
}

template<typename C, int N, typename T>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)())
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)()>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1)>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2)>(runFn);

    TypeChecker<T (C::*)(Param1, Param2)>::registerType();
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3)>(runFn);
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3, Param4)>(runFn);

}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4, Param5))
{
    if(wrapper) delete wrapper;

    wrapper = new MemberFuncWrapper<T (C::*)(Param1, Param2, Param3, Param4, Param5)>(runFn);
}

template<typename C, int N, typename T>
void AbstractTask<C, N, T>::waitForFinished()
{
    watcher.waitForFinished();
}

template<typename C, int N, typename T>
void AbstractTask<C, N, T>::finishDispatcher()
{
    emit finished(N, QVariant(watcher.result()));
}

template<typename C, int N, typename T> const int AbstractTask<C, N, T>::type;
}

#endif // TASKTEMPLATE_H
