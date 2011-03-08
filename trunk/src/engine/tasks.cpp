#include "tasks.h"
#include "../context/context.h"
#include "../gui/custom/navigationitem.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

using namespace DataEngine;

int i = qRegisterMetaType<QSqlRecord>("QSqlRecord");                    /* FillAccountsListModelTask */
int j = qRegisterMetaType< QList<QSqlRecord> >("QList<QSqlRecord>");    /* FillNavigationTreeTask */

void InitializeDBTask::run(const QString &dbPath)
{
    //asyncRun(&InitializeDBTask::initializeDB, dbPath);
}

bool InitializeDBTask::initializeDB(const QString &dbPath)
{
    return createConnection(dbPath) && createTable() && fillInitialData();
}

bool InitializeDBTask::createConnection(const QString &dbPath)
{
    bool success = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    if(db.isValid())
    {
        db.setDatabaseName(dbPath);
        success = db.open();
    }

    return success;
}

bool InitializeDBTask::createTable()
{
    static const QString createGradeClassTable = tr(
            "CREATE TABLE IF NOT EXISTS GradeClass ("
                "grade INTEGER NOT NULL,"
                "class INTEGER NOT NULL,"
                "type NVARCHAR(20) DEFAULT \"普通班\","
                "PRIMARY KEY(grade, class))"
            );
    static const QString createStudentsTable = tr(
            "CREATE TABLE IF NOT EXISTS Students ("
                "student_id INTEGER NOT NULL,"
                "sex NCHAR(1) NOT NULL,"
                "name NVARCHAR(15) NOT NULL,"
                "grade INTEGER,"
                "class INTEGER,"
                "in_school INTEGER NOT NULL DEFAULT 1,"
                "PRIMARY KEY(student_id),"
                "FOREIGN KEY(grade, class) REFERENCES GradeClass(grade, class),"
                "CHECK(sex IN ('男', '女')))"
            );
    static const QString createExamsTable = tr(
            "CREATE TABLE IF NOT EXISTS Exams ("
                "exam_id INTEGER NOT NULL,"
                "name NVARCHAR(20),"
                "date DATE NOT NULL DEFAULT CURRENT_DATE,"
                "PRIMARY KEY(exam_id))"
            );
    static const QString createCoursesTable = tr(
            "CREATE TABLE IF NOT EXISTS Courses ("
                "course_id INTEGER NOT NULL,"
                "exam_id INTEGER NOT NULL,"
                "name NVARCHAR(10),"
                "full_mark DECIMAL(10,2) DEFAULT 100,"
                "pass_rate FLOAT(2) DEFAULT 0.6 CHECK(pass_rate >=0 and pass_rate <= 1),"
                "good_rate FLOAT(2) DEFAULT 0.8 CHECK(good_rate >=0 and good_rate <= 1),"
                "excellent_rate FLOAT(2) DEFAULT 0.9 CHECK(excellent_rate >=0 and excellent_rate <= 1),"
                "PRIMARY KEY(course_id),"
                "FOREIGN KEY(exam_id) REFERENCES Exams(exam_id))"
            );
    static const QString createScoresTable = tr(
            "CREATE TABLE IF NOT EXISTS Scores ("
                "student_id INTEGER NOT NULL,"
                "course_id INTEGER NOT NULL,"
                "exam_id INTEGER NOT NULL,"
                "score DECIMAL(10,2),"
                "FOREIGN KEY(student_id) REFERENCES Students(student_id),"
                "FOREIGN KEY(course_id) REFERENCES Courses(Course_id),"
                "FOREIGN KEY(exam_id) REFERENCES Exams(exam_id))"
            );
    static const QString createClassTypeSetTable = tr(
            "CREATE TABLE IF NOT EXISTS ClassTypeSet ("
                "name NVARCHAR(20))"
            );
    static const QString createCourseSetTable = tr(
            "CREATE TABLE IF NOT EXISTS CourseSet ("
                "name NVARCHAR(10))"
            );
    static const QString createAccountsTable = tr(
            "CREATE TABLE IF NOT EXISTS Accounts ("
                "account_id NVARCHAR(10) UNIQUE NOT NULL,"
                "account_pwd NVARCHAR(10),"
                "name NVARCHAR(10),"
                "save_password INTEGER NOT NULL DEFAULT 0,"
                "permission BLOB NOT NULL,"
                "login_time DATETIME DEFAULT (datetime(current_timestamp,'localtime')))"
            );
    static const QString createVersionTable = tr(
            "CREATE TABLE IF NOT EXISTS Version ("
                "major_version INTEGER UNIQUE NOT NULL,"
                "minor_version INTEGER UNIQUE NOT NULL)"
            );

    bool success = false;

    QSqlDatabase db = QSqlDatabase::database();

    if(db.transaction())
    {
        QSqlQuery sql(db);

        success = sql.exec(createGradeClassTable);

        if(success)
            success = sql.exec(createStudentsTable);

        if(success)
            success = sql.exec(createExamsTable);

        if(success)
            success = sql.exec(createCoursesTable);

        if(success)
            success = sql.exec(createScoresTable);

        if(success)
            success = sql.exec(createClassTypeSetTable);

        if(success)
            success = sql.exec(createCourseSetTable);

        if(success)
            success = sql.exec(createAccountsTable);

        if(success)
            success = sql.exec(createVersionTable);

        if(success)
            success = db.commit();
        else
            (void)db.rollback();

        qDebug() << QThread::currentThreadId() << "InitializeDBTask::createTable -> " << sql.lastError().text();
    }

    return success;
}

bool InitializeDBTask::fillInitialData()
{
    static const QString fillVersionNumber = tr(
            "INSERT OR REPLACE INTO Version"
            "(major_version, minor_version)"
            "VALUES"
            "(2, 0)"
            );
    static const QString fillAccount = tr(
            "INSERT OR IGNORE INTO Accounts"
            "(account_id, account_pwd, name, permission)"
            "VALUES"
            "(:id, :password, :username, :permission)"
            );

    bool success = false;

    QSqlDatabase db = QSqlDatabase::database();

    if(db.transaction())
    {
        QSqlQuery sql(db);
        QByteArray byteArray;           //Permission序列化的字节流
        Permission permission;

        success = sql.exec(fillVersionNumber);

        if(success)
            success = sql.prepare(fillAccount);

        if(success)
        {
            /* 设置管理员权限到permission */
            permission.setCreateAccount(true);
            permission.setCreateClass(false);
            permission.setCreateExam(true);
            permission.setEditScore(false);

            /* 序列化permission到字节流 */
            QDataStream ds(&byteArray, QIODevice::WriteOnly);
            ds << permission;

            sql.bindValue(":id", "Admin");
            sql.bindValue(":password", "123456");
            sql.bindValue(":username", tr("管理员"));
            sql.bindValue(":permission", byteArray);

            /* 把字节流保存到数据库 */
            success = !byteArray.isEmpty() && sql.exec();
        }

        if(success)
        {
            byteArray.clear();

            /* 设置游客权限到permission */
            permission.setCreateAccount(false);
            permission.setCreateClass(false);
            permission.setCreateExam(false);
            permission.setEditScore(false);


            /* 序列化permission到字节流 */
            QDataStream ds(&byteArray, QIODevice::WriteOnly);
            ds << permission;

            sql.bindValue(":id", "Guest");
            sql.bindValue(":password", "123456");
            sql.bindValue(":username", tr("游客"));
            sql.bindValue(":permission", byteArray);

            /* 把字节流保存到数据库 */
            success = !byteArray.isEmpty() && sql.exec();
        }

        if(success)
            success = db.commit();
        else
            (void)db.rollback();

        qDebug() << QThread::currentThreadId() << "InitializeDBTask::fillInitialData -> " << sql.lastError().text();
    }

    return success;
}

void LoginTask::run(const QString &id, const QString &pwd, bool save)
{
    //asyncRun(&LoginTask::login, id, pwd, save);
}

bool LoginTask::login(const QString &id, const QString &pwd, bool save)
{
    static const QString login = tr(
            "SELECT permission FROM Accounts WHERE "
                "account_id = :id and account_pwd = :pwd"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(login))
    {
        sql.bindValue(":id", id);
        sql.bindValue(":pwd", pwd);

        if(sql.exec() && sql.next())
        {
            QVariant result = sql.value(0);

            if(result.isValid())
            {
                success = updateSaveStateAndLoginTime(id, save);

                if(success)
                {
                    QByteArray byteArray = result.toByteArray();
                    QDataStream ds(&byteArray, QIODevice::ReadOnly);

                    ds >> Context::instance()->curAccountPermission();
                }
            }
        }
    }

    qDebug() << QThread::currentThreadId() << "LoginTask::login -> " << sql.lastError().text();

    return success;
}

bool LoginTask::updateSaveStateAndLoginTime(const QString &id, bool save)
{
    static const QString updateSaveState = tr(
            "UPDATE Accounts SET save_password = :save, login_time = datetime(current_timestamp,'localtime') "
                "WHERE account_id = :id"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(updateSaveState))
    {
        sql.bindValue(":save", save ? 1 : 0);
        sql.bindValue(":id", id);

        success = sql.exec();
    }

    qDebug() << QThread::currentThreadId() << "LoginTask::updateSaveStateAndLoginTime -> " << sql.lastError().text();

    return success;
}

void InsertOrUpdateClassTask::run(int gradeNum, int classNum, const QString &classType)
{
    //asyncRun(&InsertOrUpdateClassTask::insertOrUpdateClass, gradeNum, classNum, classType);
}

bool InsertOrUpdateClassTask::insertOrUpdateClass(int gradeNum, int classNum, const QString &classType)
{
    static const QString insertGradeClass = tr(
            "INSERT OR REPLACE INTO GradeClass(grade, class, type) "
                "VALUES (:grade, :class, :type)"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(insertGradeClass))
    {
        sql.bindValue(":grade", gradeNum);
        sql.bindValue(":class", classNum);
        sql.bindValue(":type", classType);

        success = sql.exec();
    }

    return success;
}

FillAccountsListModelTask::FillAccountsListModelTask(QObject *parent) :
    AbstractTask<FillAccountsListModelTask, FillAccountsListModel, bool>(parent)
{
    connect(this,       SIGNAL(sendData(QStandardItemModel*,QSqlRecord)),
            this,       SLOT(recvData(QStandardItemModel*,QSqlRecord)));
}

void FillAccountsListModelTask::run(QStandardItemModel *model, int max)
{
    setAsyncRunEntry(&FillAccountsListModelTask::fillAccountsListModel);
    //asyncRun(&FillAccountsListModelTask::initAccountsListModel,
    //         &FillAccountsListModelTask::fillAccountsListModel, model, max);
}

void FillAccountsListModelTask::recvData(QStandardItemModel *model, const QSqlRecord &record)
{
    QList<QStandardItem *> row;

    row.append(new QStandardItem(record.value(0).toString()));     //账号
    row.append(new QStandardItem(record.value(1).toString()));     //姓名
    row.append(new QStandardItem(record.value(2).toString()));     //密码

    QStandardItem *savePWD = new QStandardItem();
    savePWD->setData(record.value(3), Qt::DisplayRole);
    row.append(savePWD);                                           //是否记住密码

    QStandardItem *autoLogin = new QStandardItem();
    autoLogin->setData(0, Qt::DisplayRole);
    row.append(autoLogin);                                         //是否自动登陆

    model->appendRow(row);
}

bool FillAccountsListModelTask::initAccountsListModel(QStandardItemModel *model, int max)
{
    model->clear();

    return true;
}

bool FillAccountsListModelTask::fillAccountsListModel(QStandardItemModel *model, int max)
{
    static const QString listAccounts = tr(
            "SELECT account_id, name,"
                "CASE WHEN save_password = 0 "
                    "THEN '' "
                "ELSE account_pwd "
                "END AS account_pwd , save_password "
            "FROM Accounts "
                "ORDER BY login_time DESC LIMIT :max"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(listAccounts))
    {
        sql.bindValue(":max", QString::number(max));

        if(sql.exec())
        {
            while(sql.next()) emit sendData(model, sql.record());

            success = true;
        }
    }

    return success;
}

FillNavigationTreeTask::FillNavigationTreeTask(QObject *parent) :
    AbstractTask<FillNavigationTreeTask, FillNavigationTree, bool>(parent)
{
    connect(this,       SIGNAL(sendData(QTreeWidget*,QList<QSqlRecord>)),
            this,       SLOT(recvData(QTreeWidget*,QList<QSqlRecord>)));
}

void FillNavigationTreeTask::run(QTreeWidget *widget, const QString &rootName)
{
    //asyncRun(&FillNavigationTreeTask::initNavigationTree,
    //         &FillNavigationTreeTask::fillNavigationTree, widget, rootName);
}

void FillNavigationTreeTask::recvData(QTreeWidget *widget, const QList<QSqlRecord> &data)
{
    NavigationItem *lastGradeItem = NULL;
    int lastGradeNum = -1;

    for(i = 0; i < data.count(); ++i)
    {
        int currGradeNum = data.at(i).value(0).toInt();
        int currClassNum = data.at(i).value(1).toInt();

        if(currGradeNum != lastGradeNum)
        {
            lastGradeNum = currGradeNum;

            lastGradeItem = new NavigationItem(widget->topLevelItem(0), NavigationItem::Grade);
            lastGradeItem->setText(0, QString::number(currGradeNum) + tr("级"));
            lastGradeItem->setData(0, NavigationItem::ExtraData, currGradeNum);
        }

        NavigationItem *classItem = new NavigationItem(lastGradeItem, NavigationItem::Class);
        classItem->setText(0, QString::number(currClassNum) + tr("班"));
        classItem->setData(0, NavigationItem::ExtraData, currClassNum);
    }
}

bool FillNavigationTreeTask::initNavigationTree(QTreeWidget *widget, const QString &rootName)
{
    widget->clear();

    NavigationItem *root  = new NavigationItem(NavigationItem::Root);   //根结点
    root->setText(0, rootName);
    widget->addTopLevelItem(root);

    return true;
}

bool FillNavigationTreeTask::fillNavigationTree(QTreeWidget *widget, const QString &rootName)
{
    static const QString gradeClassQuery = tr(
            "SELECT grade, class from GradeClass order by grade DESC, class ASC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeClassQuery))
    {
        QList<QSqlRecord> recordSet;

        while(sql.next()) recordSet.push_back(sql.record());

        emit sendData(widget, recordSet);

        success = true;
    }

    return success;
}

FillGradeListTask::FillGradeListTask(QObject *parent) :
    AbstractTask<FillGradeListTask, FillGradeList, bool>(parent)
{
    connect(this,       SIGNAL(sendData(QTreeWidget*,QVariant)),
            this,       SLOT(recvData(QTreeWidget*,QVariant)));
}

void FillGradeListTask::run(QTreeWidget *widget, const QString &headName)
{
    //asyncRun(&FillGradeListTask::initGradeList, &FillGradeListTask::fillGradeList, widget, headName);
}

void FillGradeListTask::recvData(QTreeWidget *widget, const QVariant &data)
{
    QTreeWidgetItem *gradeItem = new QTreeWidgetItem(widget);
    gradeItem->setData(0, Qt::DisplayRole, data);
    gradeItem->setTextAlignment(0, Qt::AlignCenter);
    widget->addTopLevelItem(gradeItem);
}

bool FillGradeListTask::initGradeList(QTreeWidget *widget, const QString &headName)
{
    widget->clear();

    QTreeWidgetItem *header = widget->headerItem();

    if(header)
    {
        header->setText(0, headName);
        header->setTextAlignment(0, Qt::AlignCenter);
    }

    return true;
}

bool FillGradeListTask::fillGradeList(QTreeWidget *widget, const QString &headName)
{
    static const QString gradeQuery = tr(
            "SELECT DISTINCT grade from GradeClass order by grade DESC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeQuery))
    {
        while(sql.next()) emit sendData(widget, sql.value(0));

        success = true;
    }

    return success;
}