#include "studentmgmtframe.h"
#include "studentmgmtframe_p.h"
#include "ui_studentmgmtframe.h"

#include <QSqlField>
#include <QToolBar>

/* 注册QVariant支持类型 */
Q_DECLARE_METATYPE(QList<QSqlRecord>);

StudentMgmtModel::StudentMgmtModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

Qt::ItemFlags StudentMgmtModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool StudentMgmtModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
            else
            {
                beginResetModel();
                tableData.clear();
                tableHeaderData.clear();
                endResetModel();

                success = true;
            }
            break;
        case Qt::EditRole:
            if(index.isValid() && !tableData.isEmpty())
            {
                if(index.column() >= 0 && index.column() < tableData.first().count() &&
                        index.row() >= 0 && index.row() < tableData.count())
                {
                    tableData[index.row()].setValue(index.column(), value);
                    emit dataChanged(index, index);

                    success = true;
                }
            }
            break;
        }
    }

    return success;
}

QVariant StudentMgmtModel::data(const QModelIndex &index, int role) const
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

bool StudentMgmtModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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

QVariant StudentMgmtModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(tableData.isEmpty() || role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal && section >= 0 && section < tableHeaderData.count())
        return tableHeaderData[section];

    return QAbstractTableModel::headerData(section, orientation, role);
}

int StudentMgmtModel::rowCount(const QModelIndex &parent) const
{
    return tableData.count();
}

int StudentMgmtModel::columnCount(const QModelIndex &parent) const
{
    return tableData.isEmpty() ? 0 : tableData.first().count();
}

bool StudentMgmtModel::insertRows(int row, int count, const QModelIndex &)
{
    bool success = false;

    if(row >= 0 && row <= rowCount(QModelIndex()))
    {
        beginInsertRows(QModelIndex(), row, row);

        for(int rowIndex = row; rowIndex < count; ++ rowIndex)
        {
            QSqlField newField;
            QSqlRecord newRecord;

            for(int columnIndex = 0; columnIndex < columnCount(QModelIndex()); ++columnIndex)
                newRecord.append(newField);

            tableData.insert(rowIndex, newRecord);
        }

        endInsertRows();
    }

    return success;
}

bool StudentMgmtModel::removeRows(int row, int count, const QModelIndex &)
{
    bool success = false;

    if(row >= 0 && row < rowCount(QModelIndex()))
    {
        beginRemoveRows(QModelIndex(), row, row);

        for(int rowIndex = row; rowIndex < count; ++ rowIndex)
            tableData.removeAt(rowIndex);

        endRemoveRows();
    }

    return success;
}

StudentMgmtFramePrivate::StudentMgmtFramePrivate(StudentMgmtFrame *parent, int gradeNum, int classNum) :
    task(DataEngine::Task::instance()),
    q(parent),
    gradeNum(gradeNum),
    classNum(classNum)
{

}

void StudentMgmtFramePrivate::initializeMember()
{
    QAction *insertAct = new QAction(tr("插入行"), q);
    QAction *deleteAct = new QAction(tr("删除行"), q);

    connect(insertAct, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteRow()));

    q->ui->toobar->addAction(insertAct);
    q->ui->toobar->addAction(deleteAct);

    task->lookup<DataEngine::FillStudentMgmtModelTask>()->syncRun(
                QPointer<QAbstractTableModel>(&model), gradeNum, classNum);

    q->ui->studentView->setModel(&model);
    q->ui->studentView->hideColumn(0);
    q->ui->studentView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void StudentMgmtFramePrivate::connectSignalsAndSlots()
{

}

void StudentMgmtFramePrivate::completeConstruct()
{

}

void StudentMgmtFramePrivate::insertRow()
{
    task->lookup<DataEngine::InsertRowStudentMgmtModelTask>()->run(
                QPointer<QAbstractTableModel>(&model), gradeNum, classNum, model.rowCount());
}

void StudentMgmtFramePrivate::deleteRow()
{

}

StudentMgmtFrame::StudentMgmtFrame(int gradeNum, int classNum, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StudentMgmtFrame),
    d(new StudentMgmtFramePrivate(this, gradeNum, classNum))
{
    ui->setupUi(this);
    d->initializeMember();
    d->connectSignalsAndSlots();
    d->completeConstruct();
}

StudentMgmtFrame::~StudentMgmtFrame()
{
    delete ui;
    delete d;
}
