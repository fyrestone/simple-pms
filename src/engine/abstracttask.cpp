#include "abstracttask.h"
#include "../context/permission.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>
#include <QTreeWidgetItem>

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
                "grade_class_id INTEGER PRIMARY KEY,"
                "grade INTEGER NOT NULL,"
                "class INTEGER NOT NULL,"
                "class_type_id INTEGER NOT NULL DEFAULT 0,"
                "FOREIGN KEY(class_type_id) REFERENCES ClassTypes(class_type_id))"
            );
    static const QString createStudentsTable = tr(
            "CREATE TABLE IF NOT EXISTS Students ("
                "student_id INTEGER NOT NULL,"
                "sex NCHAR(1) NOT NULL,"
                "name NVARCHAR(15) NOT NULL,"
                "in_school INTEGER NOT NULL DEFAULT 1,"
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
    static const QString createCourseSetTable = tr(
            "CREATE TABLE IF NOT EXISTS CourseSet ("
                "name NVARCHAR(10))"
            );
    static const QString createAccountsTable = tr(
            "CREATE TABLE IF NOT EXISTS Accounts ("
                "account_id NVARCHAR(10) UNIQUE NOT NULL,"
                "account_pwd NVARCHAR(10),"
                "name NVARCHAR(10),"
                "remember_pwd INTEGER NOT NULL DEFAULT 0,"
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

        success = sql.exec(createClassTypesTable);

        if(success)
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
            success = sql.exec(createCourseSetTable);

        if(success)
            success = sql.exec(createAccountsTable);

        if(success)
            success = sql.exec(createVersionTable);

        success = db.commit();
    }

    qDebug() << "InitializeDBTask::createTable -> " << db.lastError().text();

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
    static const QString fillNormalClassType = tr(
            "INSERT OR REPLACE INTO ClassTypes"
                "(class_type_id, name)"
                "VALUES"
                "(0, \"普通\")"
            );
    static const QString fillAccount = tr(
            "INSERT OR IGNORE INTO Accounts"
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

        if(sql.prepare(fillAccount))
        {
            QDataStream ds(&byteArray, QIODevice::WriteOnly);
            ds << adminPermission;

            sql.bindValue(":id", "Admin");
            sql.bindValue(":password", "123456");
            sql.bindValue(":username", tr("管理员"));
            sql.bindValue(":permission", byteArray);

            result = !byteArray.isEmpty() && sql.exec();
        }

        if(result)
        {
            byteArray.clear();

            adminPermission.setCreateAccount(false);
            adminPermission.setCreateClass(false);
            adminPermission.setCreateExam(false);
            adminPermission.setEditScore(false);

            if(sql.prepare(fillAccount))
            {
                QDataStream ds(&byteArray, QIODevice::WriteOnly);
                ds << adminPermission;

                sql.bindValue(":id", "Guest");
                sql.bindValue(":password", "123456");
                sql.bindValue(":username", tr("游客"));
                sql.bindValue(":permission", byteArray);

                result = !byteArray.isEmpty() && sql.exec();
            }
        }
    }

    qDebug() << "InitializeDBTask::fillInitialData -> " << sql.lastError().text();

    return result;
}

void LoginTask::run(const QString &id, const QString &pwd, bool save)
{
    watchFuture(QtConcurrent::run(this, &LoginTask::login, id, pwd, save));
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
                success = updateSaveState(id, save);

                QByteArray byteArray = result.toByteArray();
                QDataStream ds(&byteArray, QIODevice::ReadOnly);
                Permission permission;

                ds >> permission;

                qDebug() <<
                        permission.canCreateAccount() <<
                        permission.canCreateClass() <<
                        permission.canCreateExam() <<
                        permission.canEditScore();

                //success = true;
            }
        }
    }

    return success;
}

bool LoginTask::updateSaveState(const QString &id, bool save)
{
    static const QString updateSaveState = tr(
            "UPDATE Accounts SET remember_pwd = :save "
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

void FillAccountsListModelTask::run(QStandardItemModel *model, int max)
{
    watchFuture(QtConcurrent::run(this, &FillAccountsListModelTask::fillAccountsListModel, model, max));
}

bool FillAccountsListModelTask::fillAccountsListModel(QStandardItemModel *model, int max)
{
    static const QString listAccounts = tr(
            "SELECT account_id, name,"
                "CASE WHEN remember_pwd = 0 "
                    "THEN '' "
                "ELSE account_pwd "
                "END AS account_pwd , remember_pwd "
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
            model->clear();

            for(int i = 0; sql.next(); ++i)
            {
                QList<QStandardItem *> row;

                /* 线程中创建的对象可以在主线程中安全删除 */

                row.append(new QStandardItem(sql.value(0).toString()));     //账号
                row.append(new QStandardItem(sql.value(1).toString()));     //姓名
                row.append(new QStandardItem(sql.value(2).toString()));     //密码
                QStandardItem *savePWD = new QStandardItem();
                savePWD->setData(sql.value(3), Qt::DisplayRole);
                row.append(savePWD);                                        //是否记住密码
                QStandardItem *autoLogin = new QStandardItem();
                autoLogin->setData(0, Qt::DisplayRole);
                row.append(autoLogin);                                      //是否自动登陆

                model->appendRow(row);
                qDebug() << model->rowCount() << sql.value(0) << sql.value(1) << sql.value(2);
            }

            success = true;
        }
    }

    qDebug() << "FillAccountsListModelTask::fillAccountsListModel -> " << sql.lastError().text();

    return success;
}

void FillClassTreeWidgetTask::run(QTreeWidget *widget)
{
    watchFuture(QtConcurrent::run(this, &FillClassTreeWidgetTask::fillClassTreeWidget, widget));
}

bool FillClassTreeWidgetTask::fillClassTreeWidget(QTreeWidget *widget)
{
    static const QString gradeClassQuery = tr(
            "SELECT grade, class from GradeClass order by grade DESC, class ASC"
            );

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeClassQuery))
    {
        QTreeWidgetItem *root  = new QTreeWidgetItem(); //根结点
        QTreeWidgetItem *lastGradeItem = NULL;          //上次年级节点
        int lastGradeNum = -1;                          //上次年级号

        root->setText(0, tr("根"));
        while(sql.next())
        {
            int gradeNum = sql.value(0).toInt();

            if(gradeNum != lastGradeNum)
            {
                lastGradeNum = gradeNum;
                lastGradeItem = new QTreeWidgetItem(root);
                lastGradeItem->setText(0, QString::number(gradeNum));
            }

            (new QTreeWidgetItem(lastGradeItem))->setText(0, sql.value(1).toString());
        }

        widget->clear();
        widget->addTopLevelItem(root);
    }

    qDebug() << "FillClassTreeWidgetTask::fillClassTreeWidget -> " << sql.lastError().text();

    return true;
}

void InsertGradeClassTask::run(int gradeNum, int classNum, int classType)
{
    watchFuture(QtConcurrent::run(this, &InsertGradeClassTask::insertGradeClass, gradeNum, classNum, classType));
}

bool InsertGradeClassTask::insertGradeClass(int gradeNum, int classNum, int classType)
{
    static const QString insertGradeClass = tr(
            "INSERT INTO GradeClass(grade, class, class_type_id) "
                "VALUES (:grade, :class, :class_type_id)"
            );

    bool success = false;

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.prepare(insertGradeClass))
    {
        sql.bindValue(":grade", gradeNum);
        sql.bindValue(":class", classNum);
        sql.bindValue(":class_type_id", classType);

        success = sql.exec();
    }

    return success;
}
