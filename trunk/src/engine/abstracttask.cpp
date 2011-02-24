#include "abstracttask.h"
#include "../context/context.h"
#include "../gui/custom/navigationitem.h"
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

        qDebug() << "InitializeDBTask::createTable -> " << sql.lastError().text();
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

        qDebug() << "InitializeDBTask::fillInitialData -> " << sql.lastError().text();
    }

    return success;
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

    qDebug() << "LoginTask::login -> " << sql.lastError().text();

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

    qDebug() << "LoginTask::updateSaveStateAndLoginTime -> " << sql.lastError().text();

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

void FillNavigationTreeTask::run(QTreeWidget *widget, const QString &rootName)
{
    watchFuture(QtConcurrent::run(this, &FillNavigationTreeTask::fillNavigationTree, widget, rootName));
}

bool FillNavigationTreeTask::fillNavigationTree(QTreeWidget *widget, const QString &rootName)
{
    static const QString gradeClassQuery = tr(
            "SELECT grade, class from GradeClass order by grade DESC, class ASC"
            );

    QSqlQuery sql(QSqlDatabase::database());

    if(sql.exec(gradeClassQuery))
    {
        NavigationItem *root  = new NavigationItem(NavigationItem::Root);   //根结点
        NavigationItem *lastGradeItem = NULL;                               //上次年级节点
        int lastGradeNum = -1;                                              //上次年级号

        root->setText(0, rootName);

        while(sql.next())
        {
            int gradeNum = sql.value(0).toInt();

            if(gradeNum != lastGradeNum)
            {
                lastGradeNum = gradeNum;
                lastGradeItem = new NavigationItem(root, NavigationItem::Grade);
                lastGradeItem->setText(0, QString::number(gradeNum) + tr("级"));
                lastGradeItem->setData(0, NavigationItem::ExtraData, gradeNum);
            }

            NavigationItem *classItem = new NavigationItem(lastGradeItem, NavigationItem::Class);
            classItem->setText(0, sql.value(1).toString() + tr("班"));
            classItem->setData(0, NavigationItem::ExtraData, sql.value(1));
        }

        widget->clear();
        widget->addTopLevelItem(root);
    }

    qDebug() << "FillClassTreeWidgetTask::fillClassTreeWidget -> " << sql.lastError().text();

    return true;
}

void InsertOrUpdateNavigationTreeTask::run(QTreeWidget *widget, int gradeNum, int classNum, const QString &classType)
{
    watchFuture(QtConcurrent::run(this, &InsertOrUpdateNavigationTreeTask::insertOrUpdateNavigationTree,
                                  widget, gradeNum, classNum, classType));
}

bool InsertOrUpdateNavigationTreeTask::insertOrUpdateNavigationTree(QTreeWidget *widget,
                                                                    int gradeNum,
                                                                    int classNum,
                                                                    const QString &classType)
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

        QTreeWidgetItem *root = widget->topLevelItem(0);

        Q_ASSERT_X(root,                                                     //断言
                   "InsertOrUpdateClassTreeTask::insertOrUpdateClassTree",   //位置
                   "Tree has not been initialized!");                        //原因
        if(root)
        {
            QTreeWidgetItem *gradeItem = NULL;

            for(int i = 0; i < root->childCount(); ++i)
            {
                QTreeWidgetItem *curGradeItem = root->child(i);

                if(curGradeItem && curGradeItem->type() == NavigationItem::Grade)
                    if(curGradeItem->data(0, NavigationItem::ExtraData).toInt() == gradeNum)
                    {
                    gradeItem = curGradeItem;
                    break;
                }
            }

            if(!gradeItem)
            {
                gradeItem = new NavigationItem(root, NavigationItem::Grade);
                gradeItem->setText(0, QString::number(gradeNum) + tr("级"));
                gradeItem->setData(0, NavigationItem::ExtraData, gradeNum);
            }

            QTreeWidgetItem *classItem = NULL;

            for(int i = 0; i < gradeItem->childCount(); ++i)
            {
                QTreeWidgetItem *curClassItem = root->child(i);

                if(curClassItem && curClassItem->type() == NavigationItem::Class)
                    if(curClassItem->data(0, NavigationItem::ExtraData).toInt() == classNum)
                    {
                    classItem = curClassItem;
                    break;
                }
            }

            if(!classItem)
            {
                classItem = new NavigationItem(gradeItem, NavigationItem::Class);
                classItem->setText(0, QString::number(classNum) + tr("班"));
                classItem->setData(0, NavigationItem::ExtraData, classNum);
            }
        }
    }

    qDebug() << "fuck:" << sql.lastError().text();

    return success;
}
