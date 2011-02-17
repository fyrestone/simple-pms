#ifndef LOGINDLG_P_H
#define LOGINDLG_P_H

#include <QtCore/QObject>               /* 使用Q_DECLARE_PUBLIC */
#include <QCompleter>                   /* 使用QCompleter */
#include <QTreeView>                    /* 使用QTreeView */
#include "../../engine/dataengine.h"    /* 使用Task */

class LoginDlg;

class LoginDlgPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(LoginDlg)

public:
    LoginDlgPrivate(LoginDlg *parent);

private slots:
    void finished(DataEngine::Tasks name, const QVariant &result);

private:
    void initializeMember();
    void connectSignalsAndSlots();

private:      
    LoginDlg * const q_ptr;
    DataEngine::Task *task;
    QTreeView treeView;
    QStandardItemModel model;
    QCompleter completer;
};

#endif // LOGINDLG_P_H
