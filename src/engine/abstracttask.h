#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include <QObject>
#include <QVariant>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QFutureSynchronizer>

class QStandardItemModel;
class QTreeWidget;

namespace DataEngine
{
    enum Tasks                              ///  任务枚举
    {
        InitializeDB,                       ///< 创建数据库连接->创建表->填充初始化数据
        Login,                              ///< 登陆
        FillAccountsListModel,              ///< 填充账号列表模型
        FillNavigationTree,                 ///< 填充班级树模型
        InsertOrUpdateNavigationTree,       ///< 插入年级、班级
        FillGradeList                       ///< 填充年级列表
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

    template<Tasks N, typename T>
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
    template<typename Class>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)())
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1), const Arg1 &arg1)
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
    void AbstractTask<N, T>::asyncRun(T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
    {
        watcher.waitForFinished();
        watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4, arg5));
    }

    template<Tasks N, typename T>
    template<typename Class>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(), T (Class::*runFn)())
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)())
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1), T (Class::*runFn)(Param1), const Arg1 &arg1)
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)(arg1))
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2), T (Class::*runFn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)(arg1, arg2))
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3), T (Class::*runFn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3))
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4), T (Class::*runFn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3, arg4))
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4));
    }

    template<Tasks N, typename T>
    template<typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
    void AbstractTask<N, T>::asyncRun(bool (Class::*initFn)(Param1, Param2, Param3, Param4, Param5), T (Class::*runFn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
    {
        watcher.waitForFinished();
        if((static_cast<Class*>(this)->*initFn)(arg1, arg2, arg3, arg4, arg5))
            watcher.setFuture(QtConcurrent::run(static_cast<Class*>(this), runFn, arg1, arg2, arg3, arg4, arg5));
    }

    template<Tasks N, typename T>
    void AbstractTask<N, T>::finishDispatcher()
    {
        emit finished(N, QVariant(watcher.result()));
    }

    class InitializeDBTask : public AbstractTask<InitializeDB, bool>
    {
    public:
        void run(const QString &dbPath);

    private:
        bool initializeDB(const QString &dbPath);
        bool createConnection(const QString &dbPath);
        bool createTable();
        bool fillInitialData();
    };

    class LoginTask : public AbstractTask<Login, bool>
    {
    public:
        void run(const QString &id, const QString &pwd, bool save);

    private:
        bool login(const QString &id, const QString &pwd, bool save);
        bool updateSaveStateAndLoginTime(const QString &id, bool save);
    };

    class FillAccountsListModelTask : public AbstractTask<FillAccountsListModel, bool>
    {
    public:
        void run(QStandardItemModel *model, int max);

    private:
        bool fillAccountsListModel(QStandardItemModel *model, int max);
    };

    class FillNavigationTreeTask : public AbstractTask<FillNavigationTree, bool>
    {
    public:
        void run(QTreeWidget *widget, const QString &rootName);

    private:
        bool fillNavigationTree(QTreeWidget *widget, const QString &rootName);
    };

    class InsertOrUpdateNavigationTreeTask : public AbstractTask<InsertOrUpdateNavigationTree, bool>
    {
    public:
        void run(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType);

    private:
        bool insertOrUpdateNavigationTree(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType);
    };

    class FillGradeListTask : public AbstractTask<FillGradeList, bool>
    {
        Q_OBJECT

    public:
        FillGradeListTask(QObject *parent = 0);

        void run(QTreeWidget *widget, const QString &headName);

    signals:
        void sendGradeNum(QTreeWidget *widget, const QVariant &data);

    private slots:
        void addGradeListItem(QTreeWidget *widget, const QVariant &data);

    private:
        bool syncInit(QTreeWidget *widget, const QString &headName);
        bool fillGradeList(QTreeWidget *widget, const QString &headName);
    };
}

#endif // ABSTRACTTASK_H
