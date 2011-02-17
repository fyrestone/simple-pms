#include "logindlg_p.h"
#include "../logindlg.h"
#include <QStandardItemModel>
#include <QTreeView>

LoginDlgPrivate::LoginDlgPrivate(LoginDlg *parent)
    : q_ptr(parent)
{
    initializeMember();
    connectSignalsAndSlots();
    task->initializeDB("fuck.db");
}

void LoginDlgPrivate::initializeMember()
{
    task = DataEngine::Task::instance();

    treeView.setRootIsDecorated(false);
    treeView.setHeaderHidden(true);

    completer.setCaseSensitivity(Qt::CaseInsensitive);
    completer.setCompletionMode(QCompleter::PopupCompletion);
}

void LoginDlgPrivate::connectSignalsAndSlots()
{
    connect(task, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SLOT(finished(DataEngine::Tasks,QVariant)));
}

void LoginDlgPrivate::finished(DataEngine::Tasks name, const QVariant &result)
{
    switch(name)
    {
    case DataEngine::InitializeDB:
        task->fillAccountsListModel(&model);
        break;
    case DataEngine::Login:
        break;
    case DataEngine::FillAccountsListModel:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            completer.setModel(&model);
            completer.setPopup(&treeView);

            treeView.header()->setStretchLastSection(false);
            treeView.header()->setResizeMode(0, QHeaderView::Stretch);
            treeView.header()->setResizeMode(1, QHeaderView::ResizeToContents);

            task->login(tr("Admin"), tr("123456"));
        }
        break;
    }
    qDebug() << "model rows:" << model.rowCount();
    qDebug() << "finished with:" << name << result;
}
