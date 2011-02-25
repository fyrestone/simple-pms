#ifndef LOGINDLG_P_H
#define LOGINDLG_P_H

#include <QtCore/QObject>               /* 使用Q_DECLARE_PUBLIC */
#include <QTreeView>                    /* 使用QTreeView */
#include <QStandardItemModel>           /* 使用QStandardItemModel */
#include "../engine/dataengine.h"       /* 使用Task */

class LoginDlgPrivate : public QObject
{
    Q_OBJECT

public:
    LoginDlgPrivate();

    QString passwordFromModel(int index) const;
    void setPasswordToModel(int index, const QString &pwd);

    bool savePasswordFromModel(int index) const;
    void setSavePasswordToModel(int index, bool state);

    bool autoLoginFromModel(int index) const;
    void setAutoLoginToModel(int index, bool state);

public:
    DataEngine::Task *task;
    QTreeView *completerPopup;
    QTreeView comboBoxView;
    QStandardItemModel model;

signals:
    void ready();
    void logined(bool success);

private slots:
    void finished(int taskID, const QVariant &result);

private:
    Q_DISABLE_COPY(LoginDlgPrivate)

    void initializeComboBoxView();
    void initializeCompleterPopup();
};

#endif // LOGINDLG_P_H
