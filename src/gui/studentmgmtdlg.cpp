#include "studentmgmtdlg.h"
#include "studentmgmtdlg_p.h"
#include "ui_studentmgmtdlg.h"

#include <QSqlField>

/* 注册QVariant支持类型 */
Q_DECLARE_METATYPE(QList<QSqlRecord>);

StudentMgmtDlgModel::StudentMgmtDlgModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

Qt::ItemFlags StudentMgmtDlgModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool StudentMgmtDlgModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = false;

    if(value.isValid())
    {
        switch(role)
        {
        case Qt::UserRole:
            if(value.type() == QVariant::nameToType("QList<QSqlRecord>"))
            {
                beginResetModel();
                tableData = value.value< QList<QSqlRecord> >();
                tableHeaderData.fill(QString("未知列"), columnCount(QModelIndex()));
                endResetModel();

                success = true;
            }
            break;
        case Qt::EditRole:
            if(index.isValid())
            {

            }
            break;
        }
    }

    return success;
}

QVariant StudentMgmtDlgModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && !tableData.isEmpty())
    {
        switch(role)
        {
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
        case Qt::DisplayRole:
        case Qt::EditRole:
            if(index.column() >= 0 && index.column() < tableData.first().count() &&
                    index.row() >= 0 && index.row() < tableData.count())
                return tableData.at(index.row()).field(index.column()).value();
        }
    }

    return QVariant();
}

bool StudentMgmtDlgModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    bool success = false;

    if(orientation == Qt::Horizontal && section >= 0 && section < tableHeaderData.count())
    {
        tableHeaderData[section] = value.toString().trimmed();
        emit headerDataChanged(orientation, section, section);

        success = true;
    }

    return success;
}

QVariant StudentMgmtDlgModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(tableData.isEmpty() || role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal && section >= 0 && section < tableHeaderData.count())
        return tableHeaderData[section];

    return QAbstractTableModel::headerData(section, orientation, role);
}

int StudentMgmtDlgModel::rowCount(const QModelIndex &parent) const
{
    return tableData.count();
}

int StudentMgmtDlgModel::columnCount(const QModelIndex &parent) const
{
    return tableData.isEmpty() ? 0 : tableData.first().count();
}

bool StudentMgmtDlgModel::insertRow(int row, const QModelIndex &parent)
{
    bool success = false;

    if(parent.isValid() && row >= 0 && row <= rowCount(QModelIndex()))
    {
        QSqlField newField;
        QSqlRecord newRecord;

        beginInsertRows(parent, row, row);

        for(int i = 0; i < columnCount(QModelIndex()); ++i)
            newRecord.append(newField);
        tableData.insert(row, newRecord);

        endInsertRows();
    }

    return success;
}

bool StudentMgmtDlgModel::removeRow(int row, const QModelIndex &parent)
{
    bool success = false;

    if(parent.isValid() && row >= 0 && row < rowCount(QModelIndex()))
    {
        beginRemoveRows(parent, row, row);
        tableData.removeAt(row);
        endRemoveRows();
    }

    return success;
}

bool StudentMgmtDlgModel::submit()
{
    beginResetModel();

    tableData.clear();
    tableHeaderData.clear();

    endResetModel();

    return true;
}

StudentMgmtDlgPrivate::StudentMgmtDlgPrivate(StudentMgmtDlg *parent) :
    task(DataEngine::Task::instance()),
    q(parent)
{

}

void StudentMgmtDlgPrivate::initializeMember()
{
    QPointer<QAbstractTableModel> tableModel = &model;

    task->lookup<DataEngine::FillStudentMgmtModelTask>()->asyncRun(tableModel, 2010, 1);
    q->ui->studentInfoView->setModel(&model);
}

void StudentMgmtDlgPrivate::connectSignalsAndSlots()
{

}

void StudentMgmtDlgPrivate::completeConstruct()
{

}

StudentMgmtDlg::StudentMgmtDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentMgmtDlg),
    d(new StudentMgmtDlgPrivate(this))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
    d->completeConstruct();
}

StudentMgmtDlg::~StudentMgmtDlg()
{
    delete ui;
    delete d;
}
