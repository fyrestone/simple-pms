#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include <QObject>
#include <QVariant>
#include <QtConcurrentRun>
#include <QFutureWatcher>

class QStandardItemModel;
class QTreeWidget;

namespace DataEngine
{
    enum Tasks                  ///  任务枚举
    {
        InitializeDB,           ///< 创建数据库连接->创建表->填充初始化数据
        Login,                  ///< 登陆
        FillAccountsListModel,  ///< 填充账号列表模型
        FillClassTreeWidget,    ///< 填充班级树模型
        InsertGradeClass        ///< 插入年级、班级
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
        bool updateSaveState(const QString &id, bool save);
    };

    class FillAccountsListModelTask : public AbstractTask<FillAccountsListModel, bool>
    {
    public:
        void run(QStandardItemModel *model, int max);

    private:
        bool fillAccountsListModel(QStandardItemModel *model, int max);
    };

    class FillClassTreeWidgetTask : public AbstractTask<FillClassTreeWidget, bool>
    {
    public:
        void run(QTreeWidget *widget);

    private:
        bool fillClassTreeWidget(QTreeWidget *widget);
    };

    class InsertGradeClassTask : public AbstractTask<InsertGradeClass, bool>
    {
    public:
        void run(int gradeNum, int classNum, int classType);

    private:
        bool insertGradeClass(int gradeNum, int classNum, int classType);
    };
}

#endif // ABSTRACTTASK_H
