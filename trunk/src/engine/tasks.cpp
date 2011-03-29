#include "tasks.h"
#include "../context/context.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

using namespace DataEngine;

/*!
测试target，若为假，打印信息并退出
\param target 测试的逻辑值
*/
#define RETURN_IF_FAIL(target)                                                  \
    if(!(target))                                                               \
    {                                                                           \
        qDebug("%s:%d Info: " #target " failed", __PRETTY_FUNCTION__, __LINE__);\
        return;                                                                 \
    }

/*!
打印函数运行线程信息
*/
#define PRINT_RUN_THREAD()                                                      \
    qDebug() << __PRETTY_FUNCTION__ << "run in Thread" << QThread::currentThreadId()

/* 注册信号/槽参数类型 */
int i1 = qRegisterMetaType<QSqlRecord>("QSqlRecord");
int i2 = qRegisterMetaType< QPointer<QStandardItemModel> >("QPointer<QStandardItemModel>");
int i3 = qRegisterMetaType< QPointer<QTreeWidget> >("QPointer<QTreeWidget>");
int i4 = qRegisterMetaType< QList<QSqlRecord> >("QList<QSqlRecord>");

InitializeDBTask::InitializeDBTask(QObject *parent) :
    AbstractTask<InitializeDBTask, InitializeDB, bool>(parent)
{
    setRunEntry(&InitializeDBTask::run);
}

bool InitializeDBTask::run(const QString &dbPath)
{
    PRINT_RUN_THREAD();

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
    static const QString createClassTypeTable = tr(
            "CREATE TABLE IF NOT EXISTS ClassType ("
                "class_type_id INTEGER PRIMARY KEY NOT NULL,"
                "name NVARCHAR(20))"
            );
    static const QString createGradeClassTable = tr(
            "CREATE TABLE IF NOT EXISTS GradeClass ("
                "grade INTEGER NOT NULL,"
                "class INTEGER NOT NULL,"
                "class_type_id INTEGER NOT NULL DEFAULT 0,"
                "PRIMARY KEY(grade, class),"
                "FOREIGN KEY(class_type_id) REFERENCES ClassType(class_type_id) ON DELETE SET DEFAULT ON UPDATE CASCADE)"
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
                "FOREIGN KEY(grade, class) REFERENCES GradeClass(grade, class) ON DELETE SET NULL ON UPDATE CASCADE,"
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
                "course_id INTEGER PRIMARY KEY NOT NULL,"
                "exam_id INTEGER NOT NULL,"
                "name NVARCHAR(10),"
                "FOREIGN KEY(exam_id) REFERENCES Exams(exam_id) ON DELETE CASCADE ON UPDATE CASCADE)"
            );
    static const QString createNormalCoursesTable = tr(
            "CREATE TABLE IF NOT EXISTS NormalCourses ("
                "course_id INTEGER NOT NULL,"
                "full_mark DECIMAL(10,2) DEFAULT 100,"
                "pass_rate FLOAT(2) DEFAULT 0.6 CHECK(pass_rate >=0 and pass_rate <= 1),"
                "good_rate FLOAT(2) DEFAULT 0.8 CHECK(good_rate >=0 and good_rate <= 1),"
                "excellent_rate FLOAT(2) DEFAULT 0.9 CHECK(excellent_rate >=0 and excellent_rate <= 1),"
                "PRIMARY KEY(course_id),"
                "FOREIGN KEY(course_id) REFERENCES Courses(course_id) ON DELETE CASCADE ON UPDATE CASCADE)"
            );
    static const QString createSpecialCoursesTable = tr(
            "CREATE TABLE IF NOT EXISTS SpecialCourses ("
                "course_id INTEGER NOT NULL,"
                "command NVARCHAR(100),"
                "PRIMARY KEY(course_id),"
                "FOREIGN KEY(course_id) REFERENCES Courses(course_id) ON DELETE CASCADE ON UPDATE CASCADE)"
            );
    static const QString createScoresTable = tr(
            "CREATE TABLE IF NOT EXISTS Scores ("
                "student_id INTEGER NOT NULL,"
                "course_id INTEGER NOT NULL,"
                "exam_id INTEGER NOT NULL,"
                "score DECIMAL(10,2),"
                "PRIMARY KEY(student_id, course_id, exam_id),"
                "FOREIGN KEY(student_id) REFERENCES Students(student_id) ON DELETE CASCADE ON UPDATE CASCADE,"
                "FOREIGN KEY(course_id) REFERENCES Courses(Course_id) ON DELETE CASCADE ON UPDATE CASCADE,"
                "FOREIGN KEY(exam_id) REFERENCES Exams(exam_id) ON DELETE CASCADE ON UPDATE CASCADE)"
            );
    static const QString createCourseSetTable = tr(
            "CREATE TABLE IF NOT EXISTS CourseNameSet ("
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

        success = sql.exec(createClassTypeTable);

        if(success)
            success = sql.exec(createGradeClassTable);

        if(success)
            success = sql.exec(createStudentsTable);

        if(success)
            success = sql.exec(createExamsTable);

        if(success)
            success = sql.exec(createCoursesTable);

        if(success)
            success = sql.exec(createNormalCoursesTable);

        if(success)
            success = sql.exec(createSpecialCoursesTable);

        if(success)
            success = sql.exec(createScoresTable);

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
    }

    return success;
}

bool InitializeDBTask::fillInitialData()
{
    static const QString fillDefaultClassType = tr(
            "INSERT OR REPLACE INTO ClassType"
                "(class_type_id, name)"
                "VALUES"
                "(0, \"普通班\")"
            );
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

        success = sql.exec(fillDefaultClassType);

        if(success)
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
    }

    return success;
}

LoginTask::LoginTask(QObject *parent) :
    AbstractTask<LoginTask, Login, bool>(parent)
{
    setRunEntry(&LoginTask::run);
}

bool LoginTask::run(const QString &id, const QString &pwd, bool save)
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

    PRINT_RUN_THREAD();

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

    return success;
}

InsertOrUpdateClassTask::InsertOrUpdateClassTask(QObject *parent) :
    AbstractTask<InsertOrUpdateClassTask, InsertOrUpdateClass, bool>(parent)
{
    setRunEntry(&InsertOrUpdateClassTask::run);
}

bool InsertOrUpdateClassTask::run(int gradeNum, int classNum, int classTypeID)
{
    static const QString insertGradeClass = tr(
            "INSERT OR REPLACE INTO GradeClass(grade, class, class_type_id) "
                "VALUES (:grade, :class, :class_type_id)"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(insertGradeClass))
    {
        sql.bindValue(":grade", gradeNum);
        sql.bindValue(":class", classNum);
        sql.bindValue(":class_type_id", classTypeID);

        success = sql.exec();
    }

    PRINT_RUN_THREAD();

    return success;
}

DeleteClassTask::DeleteClassTask(QObject *parent) :
    AbstractTask<DeleteClassTask, DeleteClass, bool>(parent)
{
    setRunEntry(&DeleteClassTask::run);
}

bool DeleteClassTask::run(int gradeNum, int classNum)
{
    static const QString deleteGradeClass = tr(
                "DELETE FROM GradeClass WHERE grade = :grade and class = :class"
                );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(deleteGradeClass))
    {
        sql.bindValue(":grade", gradeNum);
        sql.bindValue(":class", classNum);

        success = sql.exec();
    }

    PRINT_RUN_THREAD();

    return success;
}

FillAccountsListModelTask::FillAccountsListModelTask(QObject *parent) :
    AbstractTask<FillAccountsListModelTask, FillAccountsListModel, bool>(parent)
{
    setRunEntry(&FillAccountsListModelTask::run);

    connect(this,       SIGNAL(querySuccess(QPointer<QStandardItemModel>)),
            this,       SLOT(initModel(QPointer<QStandardItemModel>)));
    connect(this,       SIGNAL(sendData(QPointer<QStandardItemModel>,QSqlRecord)),
            this,       SLOT(recvData(QPointer<QStandardItemModel>,QSqlRecord)));
}

bool FillAccountsListModelTask::run(QPointer<QStandardItemModel> model, int max)
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
            emit querySuccess(model);

            while(sql.next()) emit sendData(model, sql.record());

            success = true;
        }
    }

    PRINT_RUN_THREAD();

    return success;
}

void FillAccountsListModelTask::initModel(QPointer<QStandardItemModel> model)
{
    RETURN_IF_FAIL(model);

    model->clear();
}

void FillAccountsListModelTask::recvData(QPointer<QStandardItemModel> model, const QSqlRecord &record)
{
    RETURN_IF_FAIL(model);

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

FillNavigationTreeTask::FillNavigationTreeTask(QObject *parent) :
    AbstractTask<FillNavigationTreeTask, FillNavigationTree, bool>(parent)
{
    setRunEntry(&FillNavigationTreeTask::run);

    connect(this,       SIGNAL(querySuccess(QPointer<QTreeWidget>,QString)),
            this,       SLOT(initWidget(QPointer<QTreeWidget>,QString)));
    connect(this,       SIGNAL(sendData(QPointer<QTreeWidget>,QList<QSqlRecord>)),
            this,       SLOT(recvData(QPointer<QTreeWidget>,QList<QSqlRecord>)));
}

bool FillNavigationTreeTask::run(QPointer<QTreeWidget> widget, const QString &rootName)
{
    static const QString gradeClassQuery = tr(
            "SELECT grade, class from GradeClass order by grade DESC, class ASC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeClassQuery))
    {
        QList<QSqlRecord> recordSet;

        emit querySuccess(widget, rootName);
        while(sql.next()) recordSet.push_back(sql.record());
        emit sendData(widget, recordSet);

        success = true;
    }

    PRINT_RUN_THREAD();

    return success;
}

void FillNavigationTreeTask::initWidget(QPointer<QTreeWidget> widget, const QString &rootName)
{
    RETURN_IF_FAIL(widget);

    widget->clear();

    QTreeWidgetItem *root = new QTreeWidgetItem(Root);
    root->setText(0, rootName);

    widget->addTopLevelItem(root);
}

void FillNavigationTreeTask::recvData(QPointer<QTreeWidget> widget, const QList<QSqlRecord> &data)
{
    RETURN_IF_FAIL(widget);

    QTreeWidgetItem *lastGradeItem = NULL;
    int lastGradeNum = -1;

    for(int i = 0; i < data.count(); ++i)
    {
        int currGradeNum = data.at(i).value(0).toInt();
        int currClassNum = data.at(i).value(1).toInt();

        if(currGradeNum != lastGradeNum)
        {
            lastGradeNum = currGradeNum;

            lastGradeItem = new QTreeWidgetItem(widget->topLevelItem(0), Grade);
            lastGradeItem->setText(0, QString::number(currGradeNum) + tr("级"));
            lastGradeItem->setData(0, Qt::UserRole, currGradeNum);
        }

        QTreeWidgetItem *classItem = new QTreeWidgetItem(lastGradeItem, Class);
        classItem->setText(0, QString::number(currClassNum) + tr("班"));
        classItem->setData(0, Qt::UserRole, currClassNum);
    }

    widget->expandAll();
}

FillGradeListTask::FillGradeListTask(QObject *parent) :
    AbstractTask<FillGradeListTask, FillGradeList, bool>(parent)
{
    setRunEntry(&FillGradeListTask::run);

    connect(this,       SIGNAL(querySuccess(QPointer<QTreeWidget>,QString)),
            this,       SLOT(initWidget(QPointer<QTreeWidget>,QString)));
    connect(this,       SIGNAL(sendData(QPointer<QTreeWidget>,QVariant)),
            this,       SLOT(recvData(QPointer<QTreeWidget>,QVariant)));
}

bool FillGradeListTask::run(QPointer<QTreeWidget> widget, const QString &headName)
{
    static const QString gradeQuery = tr(
            "SELECT DISTINCT grade from GradeClass order by grade ASC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeQuery))
    {
        emit querySuccess(widget, headName);

        while(sql.next()) emit sendData(widget, sql.value(0));

        success = true;
    }

    PRINT_RUN_THREAD();

    return success;
}

void FillGradeListTask::initWidget(QPointer<QTreeWidget> widget, const QString &headName)
{
    RETURN_IF_FAIL(widget);

    widget->clear();

    QTreeWidgetItem *header = widget->headerItem();

    if(header)
    {
        header->setText(0, headName);
        header->setTextAlignment(0, Qt::AlignCenter);
    }
}

void FillGradeListTask::recvData(QPointer<QTreeWidget> widget, const QVariant &data)
{
    RETURN_IF_FAIL(widget);

    QTreeWidgetItem *gradeItem = new QTreeWidgetItem(widget);

    gradeItem->setData(0, Qt::DisplayRole, data);
    gradeItem->setTextAlignment(0, Qt::AlignCenter);

    widget->addTopLevelItem(gradeItem);
}

FillClassListTask::FillClassListTask(QObject *parent) :
    AbstractTask<FillClassListTask, FillClassList, bool>(parent)
{
    setRunEntry(&FillClassListTask::run);

    connect(this,       SIGNAL(querySuccess(QPointer<QTreeWidget>,QString)),
            this,       SLOT(initWidget(QPointer<QTreeWidget>,QString)));
    connect(this,       SIGNAL(sendData(QPointer<QTreeWidget>,QVariant)),
            this,       SLOT(recvData(QPointer<QTreeWidget>,QVariant)));
}

bool FillClassListTask::run(QPointer<QTreeWidget> widget, const QString &headName, int gradeNum)
{
    static const QString classQuery = tr(
            "SELECT DISTINCT class from GradeClass WHERE grade = :gradeNum order by class ASC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(classQuery))
    {
        sql.bindValue(":gradeNum", gradeNum);

        if(sql.exec())
        {
            emit querySuccess(widget, headName);

            while(sql.next()) emit sendData(widget, sql.value(0));

            success = true;
        }
    }

    PRINT_RUN_THREAD();

    return success;
}

void FillClassListTask::initWidget(QPointer<QTreeWidget> widget, const QString &headName)
{
    RETURN_IF_FAIL(widget);

    widget->clear();

    QTreeWidgetItem *header = widget->headerItem();

    if(header)
    {
        header->setText(0, headName);
        header->setTextAlignment(0, Qt::AlignCenter);
    }
}

void FillClassListTask::recvData(QPointer<QTreeWidget> widget, const QVariant &data)
{
    RETURN_IF_FAIL(widget);

    QTreeWidgetItem *classItem = new QTreeWidgetItem(widget);

    classItem->setData(0, Qt::DisplayRole, data);
    classItem->setTextAlignment(0, Qt::AlignCenter);

    widget->addTopLevelItem(classItem);
}

FillClassTypeListModelTask::FillClassTypeListModelTask(QObject *parent) :
    AbstractTask<FillClassTypeListModelTask, FillClassTypeListModel, bool>(parent)
{
    setRunEntry(&FillClassTypeListModelTask::run);

    connect(this,       SIGNAL(querySuccess(QPointer<QStandardItemModel>)),
            this,       SLOT(initModel(QPointer<QStandardItemModel>)));
    connect(this,       SIGNAL(sendData(QPointer<QStandardItemModel>,QSqlRecord)),
            this,       SLOT(recvData(QPointer<QStandardItemModel>,QSqlRecord)));
}

bool FillClassTypeListModelTask::run(QPointer<QStandardItemModel> model)
{
    static const QString classTypeQuery = tr(
            "SELECT class_type_id, name from ClassType order by class_type_id ASC"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(classTypeQuery))
    {
        emit querySuccess(model);

        while(sql.next()) sendData(model, sql.record());

        success = true;
    }

    PRINT_RUN_THREAD();

    return success;
}

void FillClassTypeListModelTask::initModel(QPointer<QStandardItemModel> model)
{
    RETURN_IF_FAIL(model);

    model->clear();
}

void FillClassTypeListModelTask::recvData(QPointer<QStandardItemModel> model, const QSqlRecord &record)
{
    RETURN_IF_FAIL(model);

    QList<QStandardItem *> row;

    row.append(new QStandardItem(record.value(1).toString()));     //班级类型名
    row.append(new QStandardItem(record.value(0).toString()));     //班级类型序号

    model->appendRow(row);
}
