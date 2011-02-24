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
    enum Tasks                              ///  任务枚举
    {
        InitializeDB,                       ///< 创建数据库连接->创建表->填充初始化数据
        Login,                              ///< 登陆
        FillAccountsListModel,              ///< 填充账号列表模型
        FillClassTreeWidget,                ///< 填充班级树模型
        InsertOrUpdateClassTreeWidget       ///< 插入年级、班级
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
        /*
            由于QtConcurrent::run返回的QFuture不支持中断、暂停以及进度报告，
            这里为防止连续两个相同Task并发执行，只能先waitForFinished。
        */
        watcher.waitForFinished();
        watcher.setFuture(future);
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

    class FillNavigationTreeTask : public AbstractTask<FillClassTreeWidget, bool>
    {
    public:
        void run(QTreeWidget *widget, const QString &rootName);

    private:
        bool fillNavigationTree(QTreeWidget *widget, const QString &rootName);
    };

    class InsertOrUpdateNavigationTreeTask : public AbstractTask<InsertOrUpdateClassTreeWidget, bool>
    {
    public:
        void run(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType);

    private:
        bool insertOrUpdateNavigationTree(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType);
    };
}

#endif // ABSTRACTTASK_H
