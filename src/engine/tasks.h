#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

#include "tasktemplate.h"

#include <QObject>
#include <QPointer>
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
        FillGradeList,                      ///< 填充年级列表
        FillClassList,                      ///< 填充班级列表
        FillClassTypeListModel              ///< 填充班级类型列表模型
    };

    enum NavigationItemType
    {
        /* QTreeWidgetItem::ItemType的1000以下Qt保留使用 */
        Root = 1001,                        ///< 根节点
        Grade = 1002,                       ///< 年级节点
        Class = 1003                        ///< 班级节点
    };

    class InitializeDBTask : public AbstractTask<InitializeDBTask, InitializeDB, bool>
    {
    public:
        InitializeDBTask(QObject *parent = 0);

        bool run(const QString &dbPath);

    private:
        bool createConnection(const QString &dbPath);
        bool createTable();
        bool fillInitialData();
    };

    class LoginTask : public AbstractTask<LoginTask, Login, bool>
    {
    public:
        LoginTask(QObject *parent = 0);

        bool run(const QString &id, const QString &pwd, bool save);

    private:
        bool updateSaveStateAndLoginTime(const QString &id, bool save);
    };

    class InsertOrUpdateClassTask : public AbstractTask<InsertOrUpdateClassTask, InsertOrUpdateClass, bool>
    {
    public:
        InsertOrUpdateClassTask(QObject *parent = 0);

        bool run(int gradeNum, int classNum, int classTypeID = 0);
    };

    class FillAccountsListModelTask : public AbstractTask<FillAccountsListModelTask, FillAccountsListModel, bool>
    {
        Q_OBJECT

    public:
        FillAccountsListModelTask(QObject *parent = 0);

        bool run(QPointer<QStandardItemModel> model, int max);

    signals:
        void querySuccess(QPointer<QStandardItemModel> model);
        void sendData(QPointer<QStandardItemModel> model, const QSqlRecord &record);

    private slots:
        void initModel(QPointer<QStandardItemModel> model);
        void recvData(QPointer<QStandardItemModel> model, const QSqlRecord &record);
    };

    class FillNavigationTreeTask : public AbstractTask<FillNavigationTreeTask, FillNavigationTree, bool>
    {
        Q_OBJECT

    public:
        FillNavigationTreeTask(QObject *parent = 0);

        bool run(QPointer<QTreeWidget> widget, const QString &rootName);

    signals:
        void querySuccess(QPointer<QTreeWidget> widget, const QString &rootName);
        void sendData(QPointer<QTreeWidget> widget, const QList<QSqlRecord> &data);

    private slots:
        void initWidget(QPointer<QTreeWidget> widget, const QString &rootName);
        void recvData(QPointer<QTreeWidget> widget, const QList<QSqlRecord> &data);
    };

    class FillGradeListTask : public AbstractTask<FillGradeListTask, FillGradeList, bool>
    {
        Q_OBJECT

    public:
        FillGradeListTask(QObject *parent = 0);

        bool run(QPointer<QTreeWidget> widget, const QString &headName);

    signals:
        void querySuccess(QPointer<QTreeWidget> widget, const QString &headName);
        void sendData(QPointer<QTreeWidget> widget, const QVariant &data);

    private slots:
        void initWidget(QPointer<QTreeWidget> widget, const QString &headName);
        void recvData(QPointer<QTreeWidget> widget, const QVariant &data);
    };

    class FillClassListTask : public AbstractTask<FillClassListTask, FillClassList, bool>
    {
        Q_OBJECT

    public:
        FillClassListTask(QObject *parent = 0);

        bool run(QPointer<QTreeWidget> widget, const QString &headName, int gradeNum);

    signals:
        void querySuccess(QPointer<QTreeWidget> widget, const QString &headName);
        void sendData(QPointer<QTreeWidget> widget, const QVariant &data);

    private slots:
        void initWidget(QPointer<QTreeWidget> widget, const QString &headName);
        void recvData(QPointer<QTreeWidget> widget, const QVariant &data);
    };

    class FillClassTypeListModelTask : public AbstractTask<FillClassTypeListModelTask, FillClassTypeListModel, bool>
    {
        Q_OBJECT

    public:
        FillClassTypeListModelTask(QObject *parent = 0);

        bool run(QPointer<QStandardItemModel> model);

    signals:
        void querySuccess(QPointer<QStandardItemModel> model);
        void sendData(QPointer<QStandardItemModel> model, const QSqlRecord &record);

    private slots:
        void initModel(QPointer<QStandardItemModel> model);
        void recvData(QPointer<QStandardItemModel> model, const QSqlRecord &record);
    };
}

#endif // ABSTRACTTASK_H