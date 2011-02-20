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
    enum GetDataType{Password = 2, SaveCheckState = 3, LoginCheckState = 4};
    LoginDlgPrivate(LoginDlg *parent);

    QVariant getDataFromIndex(GetDataType type, int index);
    bool setDataFromIndex(GetDataType type, int index, const QVariant &value);

public slots:
    void login();

public:
    QTreeView *completerPopup;
    QTreeView comboBoxView;
    QStandardItemModel model;

private:
    void connectSignalsAndSlots();
    void initializeComboBoxView();
    void initializeCompleterPopup();

private slots:
    void finished(DataEngine::Tasks name, const QVariant &result);

private:      
    LoginDlg * const q_ptr;
    DataEngine::Task *task;
};

#endif // LOGINDLG_P_H
