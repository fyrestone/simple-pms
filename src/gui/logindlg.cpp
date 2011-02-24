#include "logindlg.h"
#include "logindlg_p.h"
#include "ui_logindlg.h"
#include "../context/context.h"
#include <QMessageBox>

LoginDlgPrivate::LoginDlgPrivate()
    : task(DataEngine::Task::instance()),
    completerPopup(new QTreeView)
{
    connect(task, SIGNAL(finished(DataEngine::Tasks,QVariant)),
            this, SLOT(finished(DataEngine::Tasks,QVariant)));

    task->initializeDB("data.db"); //初始化数据库
}

QString LoginDlgPrivate::passwordFromModel(int index) const
{
    return model.data(model.index(index, 2)).toString();
}

void LoginDlgPrivate::setPasswordToModel(int index, const QString &pwd)
{
    (void)model.setData(model.index(index, 2), pwd);
}

bool LoginDlgPrivate::savePasswordFromModel(int index) const
{
    return model.data(model.index(index, 3)).toBool();
}

void LoginDlgPrivate::setSavePasswordToModel(int index, bool state)
{
     (void)model.setData(model.index(index, 3), state);
}

bool LoginDlgPrivate::autoLoginFromModel(int index) const
{
    return model.data(model.index(index, 4)).toBool();
}

void LoginDlgPrivate::setAutoLoginToModel(int index, bool state)
{
    (void)model.setData(model.index(index, 4), state);
}

void LoginDlgPrivate::finished(DataEngine::Tasks name, const QVariant &result)
{
    switch(name)
    {
    case DataEngine::InitializeDB:
        if(result.type() == QVariant::Bool && result.toBool())
            task->fillAccountsListModel(&model);    //填充账户列表模型
        break;
    case DataEngine::Login:
        if(result.type() == QVariant::Bool && result.toBool())
            emit logined(true);     //登陆成功
        else
            emit logined(false);    //登陆失败
        break;
    case DataEngine::FillAccountsListModel:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            Q_ASSERT_X(model.columnCount() == 5,    //条件
                       "LoginDlgPrivate::finished", //位置
                       "Filled Acount List Model"); //原因

            initializeComboBoxView();
            initializeCompleterPopup();

            emit ready(); //数据加载完毕
        }
        break;
    }

    qDebug() << "finished with:" << name << result;
}

void LoginDlgPrivate::initializeComboBoxView()
{
    comboBoxView.reset();
    comboBoxView.setRootIsDecorated(false);                                   //隐藏根
    comboBoxView.setHeaderHidden(true);                                       //隐藏表头
    comboBoxView.hideColumn(2);                                               //隐藏密码列
    comboBoxView.hideColumn(3);                                               //隐藏是否保存密码列
    comboBoxView.hideColumn(4);                                               //隐藏是否自动登录列
    comboBoxView.setSelectionBehavior(QAbstractItemView::SelectRows);         //按行选择
    comboBoxView.header()->setStretchLastSection(false);                      //禁止扩展最后一列
    comboBoxView.header()->setResizeMode(0, QHeaderView::Stretch);            //ID列扩展
    comboBoxView.header()->setResizeMode(1, QHeaderView::ResizeToContents);   //姓名列收缩
}

void LoginDlgPrivate::initializeCompleterPopup()
{
    completerPopup->reset();
    completerPopup->setRootIsDecorated(false);                                //隐藏根
    completerPopup->setHeaderHidden(true);                                    //隐藏表头
    completerPopup->hideColumn(2);                                            //隐藏密码列
    completerPopup->hideColumn(3);                                            //隐藏是否保存密码列
    completerPopup->hideColumn(4);                                            //隐藏是否自动登录列
    completerPopup->setSelectionBehavior(QAbstractItemView::SelectRows);      //按行选择
    completerPopup->header()->setStretchLastSection(false);                   //禁止扩展最后一列
    completerPopup->header()->setResizeMode(0, QHeaderView::Stretch);         //ID列扩展
    completerPopup->header()->setResizeMode(1, QHeaderView::ResizeToContents);//姓名列收缩
}

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg),
    d(new LoginDlgPrivate)
{
    ui->setupUi(this);
    initializeMember();
    connectSignalsAndSlots();
}

LoginDlg::~LoginDlg()
{
    delete ui;
    delete d;
}

void LoginDlg::initializeMember()
{
    ui->idComboBox->setModel(&d->model);
    ui->idComboBox->setView(&d->comboBoxView);
    ui->idComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->idComboBox->completer()->setPopup(d->completerPopup);
}

void LoginDlg::connectSignalsAndSlots()
{
    connect(ui->idComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update(int)));
    connect(ui->savePassword, SIGNAL(clicked(bool)), this, SLOT(updateSavePassword(bool)));
    connect(ui->autoLogin, SIGNAL(clicked(bool)), this, SLOT(updateAutoLogin(bool)));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(login()));

    connect(d, SIGNAL(ready()), this, SLOT(recoverState()));
    connect(d, SIGNAL(logined(bool)), this, SLOT(afterLogin(bool)));
}

void LoginDlg::save() const
{
    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    settings.setValue(tr("LastAccount"), ui->idComboBox->currentText());
    settings.setValue(tr("AutoLogin"), ui->autoLogin->isChecked());
    settings.endGroup();
}

void LoginDlg::load()
{
    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    {
        QString lastAccount = settings.value(tr("LastAccount")).toString(); //最后一次账户
        int index = ui->idComboBox->findText(lastAccount);                  //对应于ComboBox的index

        /* 设置idComboBox */
        ui->idComboBox->setCurrentIndex(index);

        /* 若保存密码为true，设置自动登录状态 */
        if(d->savePasswordFromModel(index))
        {
            bool autoLogin = settings.value(tr("AutoLogin")).toBool();

            ui->autoLogin->setChecked(autoLogin);
            d->setAutoLoginToModel(index, autoLogin);
        }
    }
    settings.endGroup();
}

void LoginDlg::update(int index)
{
    ui->passwordLineEdit->setText(d->passwordFromModel(index));
    ui->savePassword->setChecked(d->savePasswordFromModel(index));
    ui->autoLogin->setChecked(d->autoLoginFromModel(index));
}

void LoginDlg::updateAutoLogin(bool state)
{
    /* 更新state到对应autoLogin的model */
    d->setAutoLoginToModel(ui->idComboBox->currentIndex(), state);

    /* 自动登录为true则要设置保存密码为true */
    if(state)
    {
        ui->savePassword->setChecked(true);
        d->setSavePasswordToModel(ui->idComboBox->currentIndex(), true);
    }
}

void LoginDlg::updateSavePassword(bool state)
{
    /* 更新state到对应savePassword的model */
    d->setSavePasswordToModel(ui->idComboBox->currentIndex(), state);

    /* 保存密码为false则要设置自动登录为false */
    if(!state)
    {
        ui->autoLogin->setChecked(false);
        d->setAutoLoginToModel(ui->idComboBox->currentIndex(), false);
    }
}

void LoginDlg::afterLogin(bool success)
{
    if(success)//若登陆成功
    {
        save();//保存设置
        done(QDialog::Accepted);//退出对话框
    }
    else//若登陆失败
    {
        ui->passwordLineEdit->clear();//重置密码框
        QMessageBox::information(this, tr("密码验证错误"), tr("您输入的密码不正确！"), QMessageBox::Ok);
    }

    qDebug() << "login:" << success;
}

void LoginDlg::recoverState()
{
    load();//加载设置

    if(ui->autoLogin->isChecked()) login();//若自动登录为true，登录
}

void LoginDlg::login()
{
    QString id = ui->idComboBox->currentText();         //账号
    QString pwd = ui->passwordLineEdit->text();         //密码
    bool savePassword = ui->savePassword->isChecked();  //是否保存密码

    d->task->login(id, pwd, savePassword);
}
