#ifndef LOGINDLG_P_H
#define LOGINDLG_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */
#include <QTreeView>                    /* 使用QTreeView */
#include <QStandardItemModel>           /* 使用QStandardItemModel */

class LoginDlg;

class LoginDlgPrivate : public QObject
{
    Q_OBJECT

public:
    LoginDlgPrivate(LoginDlg *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

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
    void update(int index);
    void updateSavePassword(bool state);
    void updateAutoLogin(bool state);
    void login();

private:
    Q_DISABLE_COPY(LoginDlgPrivate)
    DataEngine::Task * const task;
    LoginDlg * const q;
    QTreeView *completerPopup;
    QTreeView comboBoxView;
    QStandardItemModel model;
};

#endif // LOGINDLG_P_H
