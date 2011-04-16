#include "studentmgmtframe.h"
#include "studentmgmtframe_p.h"
#include "ui_studentmgmtframe.h"

#include <QSqlField>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>

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

StudentMgmtProxyModel::StudentMgmtProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

bool StudentMgmtProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == 0) return false;//过滤掉（隐藏）第0列

    return QSortFilterProxyModel::filterAcceptsColumn(source_column, source_parent);
}

bool StudentMgmtProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    /* 关键词匹配忽略第0列 */
    for(int column = 1; column < sourceModel()->columnCount(); ++column)
    {
        QModelIndex source_index = sourceModel()->index(source_row, column, source_parent);
        QString key = sourceModel()->data(source_index, filterRole()).toString();

        if(key.contains(filterRegExp()))
            return true;
    }

    return false;
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
    QAction *refreshAct = new QAction(QIcon(tr(":/Icon/image/table_refresh.png")), tr("刷新"), q);
    QLabel *filterLabel = new QLabel(tr("过滤："), q);
    QLineEdit *filterLineEdit = new QLineEdit(q);

    connect(insertAct,          SIGNAL(triggered()),
            this,               SLOT(insertRow()));
    connect(deleteAct,          SIGNAL(triggered()),
            this,               SLOT(deleteRow()));
    connect(refreshAct,         SIGNAL(triggered()),
            this,               SLOT(refresh()));
    connect(filterLineEdit,     SIGNAL(textChanged(QString)),
            &proxyModel,        SLOT(setFilterFixedString(QString)));               //使用QRegExp::FixedString匹配关键词

    q->ui->toobar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);                //设置按钮为图标文字并排
    q->ui->toobar->addAction(insertAct);
    q->ui->toobar->addAction(deleteAct);
    q->ui->toobar->addAction(refreshAct);
    q->ui->toobar->addWidget(filterLabel);
    q->ui->toobar->addWidget(filterLineEdit);

    task->lookup<DataEngine::FillStudentMgmtModelTask>()->asyncRun(
                QPointer<QAbstractTableModel>(&model), gradeNum, classNum);         //异步填充表模型

    proxyModel.setFilterKeyColumn(-1);                                              //设置关键词匹配所有列
    proxyModel.setDynamicSortFilter(true);                                          //源模型改变代理模型自动更新
    proxyModel.setSourceModel(&model);                                              //设置源模型

    q->ui->studentView->setModel(&proxyModel);                                      //设置代理模型到表视图
    q->ui->studentView->setItemDelegateForColumn(1, new SexDelegate(this));         //刷新模型不影响Delegate
    q->ui->studentView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);    //列扩展
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
    const QModelIndex proxyIndex = q->ui->studentView->currentIndex();
    const QModelIndex sourceIndex = proxyModel.mapToSource(proxyIndex);

    if(sourceIndex.isValid())
    {
        task->lookup<DataEngine::DeleteRowStudentMgmtModelTask>()->run(
                    QPointer<QAbstractTableModel>(&model), sourceIndex.row());
    }
}

void StudentMgmtFramePrivate::refresh()
{
    task->lookup<DataEngine::FillStudentMgmtModelTask>()->asyncRun(
                QPointer<QAbstractTableModel>(&model), gradeNum, classNum);
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
