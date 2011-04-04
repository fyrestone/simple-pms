#ifndef STUDENTMGMTDLG_P_H
#define STUDENTMGMTDLG_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */
#include <QAbstractTableModel>          /* 继承自QAbstractTableModel */

class StudentMgmtDlg;

class StudentMgmtDlgModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StudentMgmtDlgModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool insertRow(int row, const QModelIndex &parent);
    bool removeRow(int row, const QModelIndex &parent);

public slots:
    bool submit();

private:
    Q_DISABLE_COPY(StudentMgmtDlgModel)
    QList<QSqlRecord> tableData;
    QVector<QString> tableHeaderData;
};

class StudentMgmtDlgPrivate : public QObject
{
    Q_OBJECT

public:
    StudentMgmtDlgPrivate(StudentMgmtDlg *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

private:
    Q_DISABLE_COPY(StudentMgmtDlgPrivate)
    DataEngine::Task * const task;
    StudentMgmtDlg * const q;
    StudentMgmtDlgModel model;
};

#endif // STUDENTMGMTDLG_P_H
