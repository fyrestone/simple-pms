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

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private:
    void initializeMember();
    void connectSignalsAndSlots();
    void save() const;
    void load();

private slots:
    void update(int index);
    void updateSavePassword(bool state);
    void updateAutoLogin(bool state);
    void afterLogin(bool success);
    void recoverState();
    void login();

private:
    Q_DISABLE_COPY(LoginDlg)
    Ui::LoginDlg * const ui;
    LoginDlgPrivate * const d;
};

#endif // LOGINDLG_H
