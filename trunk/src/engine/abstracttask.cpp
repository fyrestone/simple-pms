#include "abstracttask.h"
#include "../context/permission.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

using namespace DataEngine;

/* 元类型声明 */
int i = qRegisterMetaType<Tasks>("DataEngine::Tasks");

void InitializeDBTask::run(const QString &dbPath)
{
    watchFuture(QtConcurrent::run(this, &InitializeDBTask::initializeDB, dbPath));
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
    static const QString createClassTypesTable = tr(
            "CREATE TABLE IF NOT EXISTS ClassTypes ("
                "class_type_id INTEGER NOT NULL,"
                "name NVARCHAR(20),"
                "PRIMARY KEY(class_type_id))"
           );
    static const QString createGradeClassTable = tr(
            "CREATE TABLE IF NOT EXISTS GradeClass ("
                "grade_class_id INTEGER NOT NULL,"
                "grade INTEGER NOT NULL,"
                "class INTEGER NOT NULL,"
                "class_type_id INTEGER NOT NULL DEFAULT 0,"
                "PRIMARY KEY(grade_class_id),"
                "FOREIGN KEY(class_type_id) REFERENCES ClassTypes(class_type_id))"
            );
    static const QString createStudentsTable = tr(
            "CREATE TABLE IF NOT EXISTS Students ("
                "student_id INTEGER NOT NULL,"
                "sex NCHAR(1) NOT NULL,"
                "name NVARCHAR(15) NOT NULL,"
                "in_school INTEGER NOT NULL,"
                "info_id INTEGER,"
                "grade_class_id INTEGER NOT NULL,"
                "PRIMARY KEY(student_id),"
                "FOREIGN KEY(grade_class_id) REFERENCES GradeClass(grade_class_id),"
                "CHECK(sex IN ('男', '女')))"
            );
    static const QString createExamsTable = tr(
            "CREATE TABLE IF NOT EXISTS Exams ("
                "exam_id INTEGER NOT NULL,"
                "name NVARCHAR(20),"
                "date DATETIME NOT NULL,"
                "PRIMARY KEY(exam_id))"
            );
    static const QString createCoursesTable = tr(
            "CREATE TABLE IF NOT EXISTS Courses ("
                "course_id INTEGER NOT NULL,"
                "exam_id INTEGER NOT NULL,"
                "name NVARCHAR(10),"
                "full_mark DECIMAL(10,2),"
                "pass_rate FLOAT(1) CHECK(pass_rate >=0 and pass_rate <= 1),"
                "good_rate FLOAT(1) CHECK(good_rate >=0 and good_rate <= 1),"
                "excellent_rate FLOAT(1) CHECK(excellent_rate >=0 and excellent_rate <= 1),"
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
    static const QString createCourseSetTable = tr(
            "CREATE TABLE IF NOT EXISTS CourseSet ("
                "name NVARCHAR(10))"
            );
    static const QString createAccountsTable = tr(
            "CREATE TABLE IF NOT EXISTS Accounts ("
                "account_id NVARCHAR(10) UNIQUE NOT NULL,"
                "account_pwd NVARCHAR(10),"
                "name NVARCHAR(10),"
                "permission BLOB NOT NULL,"
                "login_time DATETIME DEFAULT (datetime(current_timestamp,'localtime')))"
            );
    static const QString createVersionTable = tr(
            "CREATE TABLE IF NOT EXISTS Version ("
                "major_version INTEGER UNIQUE NOT NULL,"
                "minor_version INTEGER UNIQUE NOT NULL)"
            );

    QSqlQuery sql(QSqlDatabase::database());

    bool result = sql.exec(createClassTypesTable);

    if(result)
        result = sql.exec(createGradeClassTable);

    if(result)
        result = sql.exec(createStudentsTable);

    if(result)
        result = sql.exec(createExamsTable);

    if(result)
        result = sql.exec(createCoursesTable);

    if(result)
        result = sql.exec(createScoresTable);

    if(result)
        result = sql.exec(createCourseSetTable);

    if(result)
        result = sql.exec(createAccountsTable);

    if(result)
        result = sql.exec(createVersionTable);

    qDebug() << "CreateTableTask::createTable -> " << sql.lastError().text();

    return result;
}

bool InitializeDBTask::fillInitialData()
{
    static const QString fillVersionNumber = tr(
            "INSERT OR REPLACE INTO Version"
                "(major_version, minor_version)"
                "VALUES"
                "(2, 0)"
            );
    static const QString fillNormalClassType = tr(
            "INSERT OR REPLACE INTO ClassTypes"
                "(class_type_id, name)"
                "VALUES"
                "(0, \"普通\")"
            );
    static const QString fillAdminAccount = tr(
            "INSERT OR REPLACE INTO Accounts"
                "(account_id, account_pwd, name, permission)"
                "VALUES"
                "(:id, :password, :username, :permission)"
            );

    QSqlQuery sql(QSqlDatabase::database());

    bool result = sql.exec(fillVersionNumber);

    if(result)
        result = sql.exec(fillNormalClassType);

    if(result)
    {
        QByteArray byteArray;
        Permission adminPermission;

        adminPermission.setCreateAccount(true);
        adminPermission.setCreateClass(false);
        adminPermission.setCreateExam(true);
        adminPermission.setEditScore(false);

        if(sql.prepare(fillAdminAccount))
        {
            QDataStream ds(&byteArray, QIODevice::WriteOnly);
            ds << adminPermission;

            sql.bindValue(":id", "Admin");
            sql.bindValue(":password", "123456");
            sql.bindValue(":username", tr("管理员"));
            sql.bindValue(":permission", byteArray);

            result = !byteArray.isEmpty() && sql.exec();
        }
    }

    qDebug() << "CreateTableTask::fillInitialData -> " << sql.lastError().text();

    return result;
}

void LoginTask::run(const QString &id, const QString &pwd)
{
    watchFuture(QtConcurrent::run(this, &LoginTask::login, id, pwd));
}

bool LoginTask::login(const QString &id, const QString &pwd)
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
                QByteArray byteArray = result.toByteArray();
                QDataStream ds(&byteArray, QIODevice::ReadOnly);
                Permission permission;

                ds >> permission;

                qDebug() <<
                        permission.canCreateAccount() <<
                        permission.canCreateClass() <<
                        permission.canCreateExam() <<
                        permission.canEditScore();

                success = true;
            }
        }
    }

    return success;
}

void FillAccountsListModelTask::run(QStandardItemModel *model, int max)
{
    watchFuture(QtConcurrent::run(this, &FillAccountsListModelTask::fillAccountsListModel, model, max));
}

bool FillAccountsListModelTask::fillAccountsListModel(QStandardItemModel *model, int max)
{
    static const QString listAccounts = tr(
            "SELECT account_id, name FROM Accounts "
                "ORDER BY login_time DESC LIMIT :max"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(listAccounts))
    {
        sql.bindValue(":max", QString::number(max));

        if(sql.exec())
        {
            model->clear();

            for(int i = 0; sql.next(); ++i)
            {
                QList<QStandardItem *> row;

                row.append(new QStandardItem(sql.value(0).toString()));
                row.append(new QStandardItem(sql.value(1).toString()));

                model->appendRow(row);
                qDebug() << model->rowCount() << sql.value(0) << sql.value(1);
            }

            success = true;
        }
    }

    qDebug() << "ListAccountsTask::listAccounts -> " << sql.lastError().text();

    return success;
}
