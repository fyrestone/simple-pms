#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include "tasktemplate.h"
#include <QObject>
#include <QList>
#include <QSqlRecord>
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
        InsertOrUpdateClass,                ///< 插入或更新班级
        FillAccountsListModel,              ///< 填充账号列表模型
        FillNavigationTree,                 ///< 填充班级树模型
        FillGradeList                       ///< 填充年级列表
    };

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

    class InsertOrUpdateClassTask : public AbstractTask<InsertOrUpdateClass, bool>
    {
    public:
        void run(int gradeNum, int classNum, const QString &classType);

    private:
        bool insertOrUpdateClass(int gradeNum, int classNum, const QString &classType);
    };

    class FillAccountsListModelTask : public AbstractTask<FillAccountsListModel, bool>
    {
        Q_OBJECT

    public:
        FillAccountsListModelTask(QObject *parent = 0);

        void run(QStandardItemModel *model, int max);

    signals:
        void sendData(QStandardItemModel *model, const QSqlRecord &record);

    private slots:
        void recvData(QStandardItemModel *model, const QSqlRecord &record);

    private:
        bool initAccountsListModel(QStandardItemModel *model, int max);
        bool fillAccountsListModel(QStandardItemModel *model, int max);
    };

    class FillNavigationTreeTask : public AbstractTask<FillNavigationTree, bool>
    {
        Q_OBJECT

    public:
        FillNavigationTreeTask(QObject *parent = 0);

        void run(QTreeWidget *widget, const QString &rootName);

    signals:
        void sendData(QTreeWidget *widget, const QList<QSqlRecord> &data);

    private slots:
        void recvData(QTreeWidget *widget, const QList<QSqlRecord> &data);

    private:
        bool initNavigationTree(QTreeWidget *widget, const QString &rootName);
        bool fillNavigationTree(QTreeWidget *widget, const QString &rootName);
    };

    class FillGradeListTask : public AbstractTask<FillGradeList, bool>
    {
        Q_OBJECT

    public:
        FillGradeListTask(QObject *parent = 0);

        void run(QTreeWidget *widget, const QString &headName);

    signals:
        void sendData(QTreeWidget *widget, const QVariant &data);

    private slots:
        void recvData(QTreeWidget *widget, const QVariant &data);

    private:
        bool initGradeList(QTreeWidget *widget, const QString &headName);
        bool fillGradeList(QTreeWidget *widget, const QString &headName);
    };
}

#endif // ABSTRACTTASK_H
