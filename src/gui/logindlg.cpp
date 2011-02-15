#include "logindlg.h"
#include "ui_logindlg.h"
#include "logic/logindlg_p.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg),
    d_ptr(new LoginDlgPrivate(this))
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}
