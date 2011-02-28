#include "logindlg.h"
#include "logindlg_p.h"
#include "ui_logindlg.h"
#include "../context/context.h"
#include <QMessageBox>

LoginDlgPrivate::LoginDlgPrivate(LoginDlg *parent) :
    completerPopup(new QTreeView),
    task(DataEngine::Task::instance()),
    q(parent)
{
    connect(task, SIGNAL(finished(int,QVariant)),
            this, SLOT(finished(int,QVariant)));

    task->initializeDB("data.db"); //初始化数据库
}

void LoginDlgPrivate::update(int index)
{
    q->ui->passwordLineEdit->setText(passwordFromModel(index));
    q->ui->savePassword->setChecked(savePasswordFromModel(index));
    q->ui->autoLogin->setChecked(autoLoginFromModel(index));
}

void LoginDlgPrivate::updateAutoLogin(bool state)
{
    /* 更新state到对应autoLogin的model */
    setAutoLoginToModel(q->ui->idComboBox->currentIndex(), state);

    /* 自动登录为true则要设置保存密码为true */
    if(state)
    {
        q->ui->savePassword->setChecked(true);
        setSavePasswordToModel(q->ui->idComboBox->currentIndex(), true);
    }
}

void LoginDlgPrivate::updateSavePassword(bool state)
{
    /* 更新state到对应savePassword的model */
    setSavePasswordToModel(q->ui->idComboBox->currentIndex(), state);

    /* 保存密码为false则要设置自动登录为false */
    if(!state)
    {
        q->ui->autoLogin->setChecked(false);
        setAutoLoginToModel(q->ui->idComboBox->currentIndex(), false);
    }
}

void LoginDlgPrivate::login()
{
    QString id = q->ui->idComboBox->currentText();         //账号
    QString pwd = q->ui->passwordLineEdit->text();         //密码
    bool savePassword = q->ui->savePassword->isChecked();  //是否保存密码

    task->login(id, pwd, savePassword);
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

void LoginDlgPrivate::save() const
{
    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    settings.setValue(tr("LastAccount"), q->ui->idComboBox->currentText());
    settings.setValue(tr("AutoLogin"), q->ui->autoLogin->isChecked());
    settings.endGroup();
}

void LoginDlgPrivate::load()
{
    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    {
        QString lastAccount = settings.value(tr("LastAccount")).toString(); //最后一次账户
        int index = q->ui->idComboBox->findText(lastAccount);               //对应于ComboBox的index

        /* 设置idComboBox */
        q->ui->idComboBox->setCurrentIndex(index);

        /* 若保存密码为true，设置自动登录状态 */
        if(savePasswordFromModel(index))
        {
            bool autoLogin = settings.value(tr("AutoLogin")).toBool();

            q->ui->autoLogin->setChecked(autoLogin);
            setAutoLoginToModel(index, autoLogin);
        }
    }
    settings.endGroup();
}

void LoginDlgPrivate::finished(int taskID, const QVariant &result)
{
    switch(taskID)
    {
    case DataEngine::InitializeDB:
        if(result.type() == QVariant::Bool && result.toBool())
            task->fillAccountsListModel(&model);        //填充账户列表模型
        break;
    case DataEngine::Login:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            save();                                     //保存设置
            q->done(QDialog::Accepted);                 //退出对话框
        }
        else
        {
            q->ui->passwordLineEdit->clear();           //重置密码框
            QMessageBox::information(q,
                                     tr("密码验证错误"),
                                     tr("您输入的密码不正确！"),
                                     QMessageBox::Ok);
        }
        break;
    case DataEngine::FillAccountsListModel:
        if(result.type() == QVariant::Bool && result.toBool())
        {
            Q_ASSERT_X(model.columnCount() == 5,        //条件
                       "LoginDlgPrivate::finished",     //位置
                       "Filled Acount List Model");     //原因

            initializeComboBoxView();
            initializeCompleterPopup();

            load();                                     //加载设置

            if(q->ui->autoLogin->isChecked()) login();  //若自动登录为true，登录
        }
        break;
    }

    qDebug() << "finished with:" << taskID << result;
}

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg),
    d(new LoginDlgPrivate(this))
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
    connect(ui->idComboBox,     SIGNAL(currentIndexChanged(int)),
            d,                  SLOT(update(int)));
    connect(ui->savePassword,   SIGNAL(clicked(bool)),
            d,                  SLOT(updateSavePassword(bool)));
    connect(ui->autoLogin,      SIGNAL(clicked(bool)),
            d,                  SLOT(updateAutoLogin(bool)));
    connect(ui->loginButton,    SIGNAL(clicked()),
            d,                  SLOT(login()));
}