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
    Q_DECLARE_PRIVATE(LoginDlg)

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private:
    Ui::LoginDlg *ui;
    LoginDlgPrivate * const d_ptr;
};

#endif // LOGINDLG_H
