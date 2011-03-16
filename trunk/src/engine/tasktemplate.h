/*!
\file tasktemplate.h
\author LiuBao
\version 1.0
\date 2011/3/14
\brief 多线程任务的模板基类

本类作为业务逻辑框架的核心，为众多业务类提供同步和异步支持。
*/
#ifndef TASKTEMPLATE_H
#define TASKTEMPLATE_H

#include <QDebug>   /* 使用qWarning */

namespace DataEngine
{

#define ENGINE_ASYNC_FUNC_NAME  asyncRun        ///<  异步接口函数名
#define ENGINE_SYNC_FUNC_NAME   syncRun         ///<  同步接口函数名

/// 打印未调用setRunEntry的警告信息，与编译器有关
#define PRINT_NOT_SET_RUN_ENTRY_WARNING() \
    qWarning("please setRunEntry before call %s", __PRETTY_FUNCTION__)

/// 打印参数类型错误并中止执行，与编译器有关
#define PRINT_PARAMETER_RUN_TIME_ERROR() \
    qCritical("pass error arguments in function: %s", __PRETTY_FUNCTION__)

/*!
\class WrapperBase
\brief 成员函数指针包装类的基类

使用本类指针（基类指针），存放各成员函数包装类（子类 DataEngine::MemberFuncWrapper ）的地址。
*/
class WrapperBase
{

};

/*!
\class MemberFuncWrapper
\brief 成员函数指针包装类

用于存放各种成员函数指针，只可在构造时存入，存入后只能够使用成员函数指针，不可修改成员函数指针。
*/
template<typename FuncType>
class MemberFuncWrapper : public WrapperBase
{
public:
    /*!
      用成员函数指针构造一个对应类型的成员函数指针包装类
      \param fnPtr 成员函数指针，其类型由类模板参数决定
     */
    MemberFuncWrapper(FuncType fnPtr) :
        ptr(fnPtr) {}

    /*!
      获取成员函数指针
      \return 成员函数指针
     */
    FuncType funcPtr() const {return ptr;}

private:
    FuncType ptr;               ///< 成员函数指针
};      

/*!
\class TypeChecker
\brief 类型检查模板类
\attention 生成的各种类型的检查类通常只用于类型检查，比较冗余。

利用同一个类型的类公用同一静态成员的原理，检查类模板参数类型是否已经存在。
*/
template<typename TypeType>
class TypeChecker
{
public:
    /*! 注册TypeType类型 */
    static void registerType()
    { ++counter; }

    /*! 检查类型TypeType是否已注册过 */
    static bool hasRegistered()
    { return counter; }

    /*! 获取类型唯一标识符 */
    static void *uuid()
    { return &counter; }

private:
    static int counter;         ///< 类型注册次数计数
};

template<typename TypeType>
int TypeChecker<TypeType>::counter = 0;

/*!
\class TypeCleaner<T>
\brief 基本类型清理模板

用于除去类型的const或者&限定符。
*/
template<typename T>
class TypeCleaner
{
public:
    typedef T ArgT;
    typedef T BareT;
    typedef T const ConstT;
    typedef T& RefT;
    typedef T& RefBareT;
    typedef T const& RefConstT;
};

/*!
\class TypeCleaner<T const>
\brief 带const修饰符类型的特化版本

针对const类型的局部特化。
*/
template<typename T>
class TypeCleaner<T const>
{
public:
    typedef T const ArgT;
    typedef T BareT;
    typedef T const ConstT;
    typedef T const& RefT;
    typedef T& RefBareT;
    typedef T const& RefConstT;
};

/*!
\class TypeCleaner<T&>
\brief 带&修饰符类型的特化版本

针对引用类型的局部特化。
*/
template<typename T>
class TypeCleaner<T&>
{
public:
    typedef T & ArgT;
    typedef typename TypeCleaner<T>::BareT BareT;
    typedef T const ConstT;
    typedef T & RefT;
    typedef typename TypeCleaner<T>::BareT & RefBareT;
    typedef T const & RefConstT;
};

/*!
\class TypeCleaner<>
\brief 空类型的特化版本

针对void的全局特化，因为指向void的引用或const类型不允许。
*/
template<>
class TypeCleaner<void>
{
public:
    typedef void ArgT;
    typedef void BareT;
    typedef void const ConstT;
    typedef void RefT;
    typedef void RefBareT;
    typedef void RefConstT;
};

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
    void *lastChecker;
};

template<typename C, int N, typename T>
AbstractTask<C, N, T>::AbstractTask(QObject *parent)
    :AbstractTaskBase(parent), wrapper(0), lastChecker(0)
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
    typedef T (C::*FuncType)();

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr()));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1)
{
    typedef T (C::*FuncType)(Arg1);

    qDebug() << TypeChecker<FuncType>::uuid() << lastChecker;

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2)
{
    typedef T (C::*FuncType)(Arg1, Arg2);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

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
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3, Arg4);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline void AbstractTask<C, N, T>::ENGINE_ASYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3, Arg4, Arg5);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        watcher.waitForFinished();
        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            watcher.setFuture(QtConcurrent::run(static_cast<C*>(this), fnWrapper->funcPtr(), arg1, arg2, arg3, arg4, arg5));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME()
{
    typedef T (C::*FuncType)();

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))());
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1)
{
    typedef T (C::*FuncType)(Arg1);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2)
{
    typedef T (C::*FuncType)(Arg1, Arg2);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3, Arg4);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3, arg4));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline void AbstractTask<C, N, T>::ENGINE_SYNC_FUNC_NAME(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    typedef T (C::*FuncType)(Arg1, Arg2, Arg3, Arg4, Arg5);

    if(TypeChecker<FuncType>::hasRegistered() && TypeChecker<FuncType>::uuid() == lastChecker)
    {
        typedef const MemberFuncWrapper<FuncType> *WrapperType;

        WrapperType fnWrapper = static_cast<WrapperType>(wrapper);
        if(fnWrapper)
            emit finished(N, (static_cast<C*>(this)->*(fnWrapper->funcPtr()))(arg1, arg2, arg3, arg4, arg5));
        else
            PRINT_NOT_SET_RUN_ENTRY_WARNING();
    }
    else
        PRINT_PARAMETER_RUN_TIME_ERROR();
}

template<typename C, int N, typename T>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)())
{
    typedef T (C::*FuncType)();

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<FuncType>::registerType();
    lastChecker = TypeChecker<FuncType>::uuid();
}

template<typename C, int N, typename T>
template<typename Param1>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1))
{
    typedef typename TypeCleaner<Param1>::BareT CleanedParam1;
    typedef T (C::*FuncType)(Param1);
    typedef T (C::*CleanedFuncType)(CleanedParam1);

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<CleanedFuncType>::registerType();
    lastChecker = TypeChecker<CleanedFuncType>::uuid();

    qDebug() << TypeChecker<CleanedFuncType>::uuid() << lastChecker;
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2))
{
    typedef typename TypeCleaner<Param1>::BareT CleanedParam1;
    typedef typename TypeCleaner<Param2>::BareT CleanedParam2;
    typedef T (C::*FuncType)(Param1, Param2);
    typedef T (C::*CleanedFuncType)(CleanedParam1, CleanedParam2);

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<CleanedFuncType>::registerType();
    lastChecker = TypeChecker<CleanedFuncType>::uuid();
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3))
{
    typedef typename TypeCleaner<Param1>::BareT CleanedParam1;
    typedef typename TypeCleaner<Param2>::BareT CleanedParam2;
    typedef typename TypeCleaner<Param3>::BareT CleanedParam3;
    typedef T (C::*FuncType)(Param1, Param2, Param3);
    typedef T (C::*CleanedFuncType)(CleanedParam1, CleanedParam2, CleanedParam3);

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<CleanedFuncType>::registerType();
    lastChecker = TypeChecker<CleanedFuncType>::uuid();
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4))
{
    typedef typename TypeCleaner<Param1>::BareT CleanedParam1;
    typedef typename TypeCleaner<Param2>::BareT CleanedParam2;
    typedef typename TypeCleaner<Param3>::BareT CleanedParam3;
    typedef typename TypeCleaner<Param4>::BareT CleanedParam4;
    typedef T (C::*FuncType)(Param1, Param2, Param3, Param4);
    typedef T (C::*CleanedFuncType)(CleanedParam1, CleanedParam2, CleanedParam3, CleanedParam4);

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<CleanedFuncType>::registerType();
    lastChecker = TypeChecker<CleanedFuncType>::uuid();
}

template<typename C, int N, typename T>
template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
inline void AbstractTask<C, N, T>::setRunEntry(T (C::*runFn)(Param1, Param2, Param3, Param4, Param5))
{
    typedef typename TypeCleaner<Param1>::BareT CleanedParam1;
    typedef typename TypeCleaner<Param2>::BareT CleanedParam2;
    typedef typename TypeCleaner<Param3>::BareT CleanedParam3;
    typedef typename TypeCleaner<Param4>::BareT CleanedParam4;
    typedef typename TypeCleaner<Param5>::BareT CleanedParam5;
    typedef T (C::*FuncType)(Param1, Param2, Param3, Param4, Param5);
    typedef T (C::*CleanedFuncType)(CleanedParam1, CleanedParam2, CleanedParam3, CleanedParam4, CleanedParam5);

    if(wrapper) delete wrapper;
    wrapper = new MemberFuncWrapper<FuncType>(runFn);

    TypeChecker<CleanedFuncType>::registerType();
    lastChecker = TypeChecker<CleanedFuncType>::uuid();
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
