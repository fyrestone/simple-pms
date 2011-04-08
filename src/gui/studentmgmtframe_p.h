#ifndef STUDENTMGMTFRAME_P_H
#define STUDENTMGMTFRAME_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */
#include <QAbstractTableModel>          /* 继承自QAbstractTableModel */

class StudentMgmtFrame;

class StudentMgmtModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StudentMgmtModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());

private:
    Q_DISABLE_COPY(StudentMgmtModel)
    QList<QSqlRecord> tableData;
    QVector<QString> tableHeaderData;
};

class StudentMgmtFramePrivate : public QObject
{
    Q_OBJECT

public:
    StudentMgmtFramePrivate(StudentMgmtFrame *parent, int gradeNum, int classNum);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

private slots:
    void insertRow();
    void deleteRow();

private:
    Q_DISABLE_COPY(StudentMgmtFramePrivate)
    DataEngine::Task * const task;
    StudentMgmtFrame * const q;
    StudentMgmtModel model;
    const int gradeNum;
    const int classNum;
};

#endif // STUDENTMGMTFRAME_P_H
