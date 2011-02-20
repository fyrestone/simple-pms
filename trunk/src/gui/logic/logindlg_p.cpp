#include "logindlg_p.h"
#include "../logindlg.h"
#include <QStandardItemModel>
#include <QTreeView>

LoginDlgPrivate::LoginDlgPrivate(LoginDlg *parent)
    : completerPopup(new QTreeView), q_ptr(parent), task(DataEngine::Task::instance())
{
    connectSignalsAndSlots();
    task->initializeDB("data.db");
}

void LoginDlgPrivate::connectSignalsAndSlots()
{
    connect(task, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SLOT(finished(DataEngine::Tasks,QVariant)));
}

void LoginDlgPrivate::initializeComboBoxView()
{
    comboBoxView.reset();
    comboBoxView.setRootIsDecorated(false);                                   //隐藏根
    comboBoxView.setHeaderHidden(true);                                       //隐藏表头
    comboBoxView.hideColumn(2);                                               //隐藏密码列
    comboBoxView.hideColumn(3);                                               //隐藏是否保存密码列
    comboBoxView.hideColumn(4);                                               //隐藏是否自动登录列
    comboBoxView.setSelectionBehavior(QAbstractItemView::SelectRows);         //按行选择
    comboBoxView.header()->setStretchLastSection(false);                      //禁止扩展最后一列
    comboBoxView.header()->setResizeMode(0, QHeaderView::Stretch);            //ID列扩展
    comboBoxView.header()->setResizeMode(1, QHeaderView::ResizeToContents);   //姓名列收缩
}

void LoginDlgPrivate::initializeCompleterPopup()
{
    completerPopup->reset();
    completerPopup->setRootIsDecorated(false);                                //隐藏根
    completerPopup->setHeaderHidden(true);                                    //隐藏表头
    completerPopup->hideColumn(2);                                            //隐藏密码列
    completerPopup->hideColumn(3);                                            //隐藏是否保存密码列
    completerPopup->hideColumn(4);                                            //隐藏是否自动登录列
    completerPopup->setSelectionBehavior(QAbstractItemView::SelectRows);      //按行选择
    completerPopup->header()->setStretchLastSection(false);                   //禁止扩展最后一列
    completerPopup->header()->setResizeMode(0, QHeaderView::Stretch);         //ID列扩展
    completerPopup->header()->setResizeMode(1, QHeaderView::ResizeToContents);//姓名列收缩
}

QVariant LoginDlgPrivate::getDataFromIndex(GetDataType type, int index)
{
    return model.data(model.index(index, (int)type));
}

bool LoginDlgPrivate::setDataFromIndex(GetDataType type, int index, const QVariant &value)
{
    return model.setData(model.index(index, (int)type), value);
}

void LoginDlgPrivate::login()
{
    Q_Q(LoginDlg);

    task->login(q->id(), q->pwd(), q->isSavePWD());
}

void LoginDlgPrivate::finished(DataEngine::Tasks name, const QVariant &result)
{   
    switch(name)
    {
    case DataEngine::InitializeDB:
        if(result.type() == QVariant::Bool && result.toBool())
            task->fillAccountsListModel(&model);
        break;
    case DataEngine::Login:
        if(result.type() == QVariant::Bool && result.toBool())
            qDebug() << "login success!";
        else
            qDebug() << "login failed!";
        break;
    case DataEngine::FillAccountsListModel:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            Q_ASSERT_X(model.columnCount() == 5, "LoginDlgPrivate::finished", "Filled Acount List Model");

            initializeComboBoxView();
            initializeCompleterPopup();

            if(model.rowCount() > 0)
            {
                Q_Q(LoginDlg);

                q->load();

                if(q->isAutoLogin())
                {
                    task->login(q->id(), q->pwd(), q->isSavePWD());
                    qDebug() << "Auto Login, please wait...";
                }
            }
        }
        break;
    }

    qDebug() << "finished with:" << name << result;
}
