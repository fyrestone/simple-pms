#ifndef LOGINDLG_P_H
#define LOGINDLG_P_H

#include <QtCore/QObject>               /* 使用Q_DECLARE_PUBLIC */
#include <QTreeView>                    /* 使用QTreeView */
#include <QStandardItemModel>           /* 使用QStandardItemModel */
#include "../engine/dataengine.h"       /* 使用Task */

class LoginDlg;

class LoginDlgPrivate : public QObject
{
    Q_OBJECT

public:
    LoginDlgPrivate(LoginDlg *parent);

public slots:
    void update(int index);
    void updateSavePassword(bool state);
    void updateAutoLogin(bool state);
    void login();

public:
    QTreeView *completerPopup;
    QTreeView comboBoxView;
    QStandardItemModel model;

private:
    void initializeComboBoxView();
    void initializeCompleterPopup();

    QString passwordFromModel(int index) const;
    void setPasswordToModel(int index, const QString &pwd);

    bool savePasswordFromModel(int index) const;
    void setSavePasswordToModel(int index, bool state);

    bool autoLoginFromModel(int index) const;
    void setAutoLoginToModel(int index, bool state);

    void save() const;
    void load();

private slots:
    void finished(int taskID, const QVariant &result);

private:
    Q_DISABLE_COPY(LoginDlgPrivate)
    DataEngine::Task *task;
    LoginDlg * const q;
};

#endif // LOGINDLG_P_H
