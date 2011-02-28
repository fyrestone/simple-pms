#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

namespace Ui {
    class LoginDlg;
}

class LoginDlgPrivate;

class LoginDlg : public QDialog
{
    Q_OBJECT

    friend class LoginDlgPrivate;

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private:
    Q_DISABLE_COPY(LoginDlg)
    Ui::LoginDlg * const ui;
    LoginDlgPrivate * const d;
};

#endif // LOGINDLG_H
