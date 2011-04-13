#include "studentmgmtframe.h"
#include "studentmgmtframe_p.h"
#include "ui_studentmgmtframe.h"

#include <QSqlField>
#include <QToolBar>

/* 注册QVariant支持类型 */
Q_DECLARE_METATYPE(QList<QSqlRecord>);

SexDelegate::SexDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

SexDelegate::~SexDelegate()
{
}

QWidget *SexDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItem(tr("男"));
    editor->addItem(tr("女"));

    return editor;
}

void SexDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QString sex = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *box = static_cast<QComboBox*>(editor);

    if(sex.isEmpty())
        box->setCurrentIndex(0);
    else
        box->setCurrentIndex(box->findText(sex));
}

void SexDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    model->setData(index, box->itemText(box->currentIndex()), Qt::EditRole);
}

void SexDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

QSize SexDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    return option.rect.size();
}

StudentMgmtModel::StudentMgmtModel(QObject *parent) :
    QAbstractTableModel(parent),
    task(DataEngine::Task::instance())
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
            if(value.type() == QVariant::nameToType("QList<QSqlRecord>"))   //更新模型
            {
                beginResetModel();
                tableData = value.value< QList<QSqlRecord> >();

                /* 若tableData的列比列标多，扩展列标（tableHeaderData） */
                if(!tableData.isEmpty() && tableData.first().count() > tableHeaderData.count())
                    tableHeaderData.resize(tableData.first().count());
                endResetModel();
            }
            else if(value.type() == QVariant::Int)                          //设置列标数
                tableHeaderData.fill(QString("未知列"), value.toInt());
            else                                                            //清空模型
            {
                beginResetModel();
                tableData.clear();
                tableHeaderData.clear();
                endResetModel();
            }
            success = true;
            break;
        case Qt::EditRole:
            if(index.isValid() && !tableData.isEmpty())
            {
                if(index.column() >= 0 && index.column() < tableData.first().count() &&
                        index.row() >= 0 && index.row() < tableData.count())
                {
                    tableData[index.row()].setValue(index.column(), value);
                    emit dataChanged(index, index);

                    if(task->lookup<DataEngine::UpdateStudentMgmtModelTask>()->run(
                                QPointer<QAbstractTableModel>(this), index, value))
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
    if(tableHeaderData.isEmpty() || role != Qt::DisplayRole)
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
    return tableHeaderData.count();
}

bool StudentMgmtModel::insertRows(int row, int count, const QModelIndex &)
{
    bool success = false;

    if(row >= 0 && row <= rowCount(QModelIndex()))
    {
        beginInsertRows(QModelIndex(), row, row);

        for(int rowIndex = row; count-- ; ++rowIndex)
        {
            QSqlField newField;
            QSqlRecord newRecord;

            /* 创建一个行（包含columnCount个列） */
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

        for(int rowIndex = row; count-- ; ++ rowIndex)
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
    QAction *insertAct = new QAction(QIcon(tr(":/Icon/image/table_row_insert.png")), tr("插入行"), q);
    QAction *deleteAct = new QAction(QIcon(tr(":/Icon/image/table_row_delete.png")), tr("删除行"), q);

    connect(insertAct, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteRow()));

    q->ui->toobar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    q->ui->toobar->addAction(insertAct);
    q->ui->toobar->addAction(deleteAct);

    task->lookup<DataEngine::FillStudentMgmtModelTask>()->syncRun(
                QPointer<QAbstractTableModel>(&model), gradeNum, classNum);

    q->ui->studentView->setModel(&model);
    q->ui->studentView->hideColumn(0);
    q->ui->studentView->setItemDelegateForColumn(2, new SexDelegate(this));
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
    const QModelIndex deleteIndex = q->ui->studentView->currentIndex();

    if(deleteIndex.isValid())
    {
        task->lookup<DataEngine::DeleteRowStudentMgmtModelTask>()->run(
                    QPointer<QAbstractTableModel>(&model), deleteIndex.row());
    }
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
