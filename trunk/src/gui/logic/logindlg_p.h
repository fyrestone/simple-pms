#ifndef LOGINDLG_P_H
#define LOGINDLG_P_H

#include <QtCore/QObject>               /* 使用Q_DECLARE_PUBLIC */
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
    LoginDlg * const q_ptr;
};

#endif // LOGINDLG_P_H
