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
    void initializeMember();
    void connectSignalsAndSlots();
    void save() const;
    void load();
    bool isSavePWD() const;
    bool isAutoLogin() const;
    QString id() const;
    QString pwd() const;

private slots:
    void setAutoLoginCheck(bool state);
    void setSavePWDCheck(bool state);
    void updateAutoLoginCheck(int index);
    void updateSavePWDCheck(int index);
    void updatePWD(int index);

private:
    Ui::LoginDlg *ui;
    LoginDlgPrivate * const d_ptr;
};

#endif // LOGINDLG_H
