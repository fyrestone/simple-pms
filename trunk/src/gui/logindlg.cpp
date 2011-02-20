#include "logindlg.h"
#include "ui_logindlg.h"
#include "logic/logindlg_p.h"
#include "../context/context.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg),
    d_ptr(new LoginDlgPrivate(this))
{
    ui->setupUi(this);
    initializeMember();
    connectSignalsAndSlots();
}

LoginDlg::~LoginDlg()
{
    save();
    delete ui;
    delete d_ptr;
}

void LoginDlg::initializeMember()
{
    Q_D(LoginDlg);

    ui->idComboBox->setModel(&d->model);
    ui->idComboBox->setView(&d->comboBoxView);
    ui->idComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->idComboBox->completer()->setPopup(d->completerPopup);
}

void LoginDlg::connectSignalsAndSlots()
{
    connect(ui->loginCheckBox, SIGNAL(clicked(bool)), this, SLOT(setAutoLoginCheck(bool)));
    connect(ui->saveCheckBox, SIGNAL(clicked(bool)), this, SLOT(setSavePWDCheck(bool)));
    connect(ui->idComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAutoLoginCheck(int)));
    connect(ui->idComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSavePWDCheck(int)));
    connect(ui->idComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePWD(int)));
    connect(ui->loginButton, SIGNAL(clicked()), d_ptr, SLOT(login()));
}

void LoginDlg::save() const
{
    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    settings.setValue(tr("LastAccount"), ui->idComboBox->currentText());
    settings.setValue(tr("AutoLogin"), ui->loginCheckBox->isChecked());
    settings.endGroup();
}

void LoginDlg::load()
{
    Q_D(LoginDlg);

    QSettings &settings = Context::instance()->curAppSettings();

    settings.beginGroup(tr("Login"));
    ui->idComboBox->setCurrentIndex(ui->idComboBox->findText(settings.value(tr("LastAccount")).toString()));
    if(d->getDataFromIndex(LoginDlgPrivate::SaveCheckState, ui->idComboBox->currentIndex()).toBool())
    {
        bool autoLogin = settings.value(tr("AutoLogin")).toBool();

        ui->loginCheckBox->setChecked(autoLogin);
        d->setDataFromIndex(LoginDlgPrivate::LoginCheckState,
                            ui->idComboBox->currentIndex(),
                            autoLogin);
    }
    settings.endGroup();
}

bool LoginDlg::isSavePWD() const
{
    return ui->saveCheckBox->isChecked();
}

bool LoginDlg::isAutoLogin() const
{
    return ui->loginCheckBox->isChecked();
}

QString LoginDlg::id() const
{
    return ui->idComboBox->currentText();
}

QString LoginDlg::pwd() const
{
    return ui->pwdLineEdit->text();
}

void LoginDlg::setAutoLoginCheck(bool state)
{
    Q_D(LoginDlg);

    d->setDataFromIndex(LoginDlgPrivate::LoginCheckState, ui->idComboBox->currentIndex(), state);
    if(state)
    {
        ui->saveCheckBox->setChecked(true);
        d->setDataFromIndex(LoginDlgPrivate::SaveCheckState, ui->idComboBox->currentIndex(), true);
    }
}

void LoginDlg::setSavePWDCheck(bool state)
{
    Q_D(LoginDlg);

    d->setDataFromIndex(LoginDlgPrivate::SaveCheckState, ui->idComboBox->currentIndex(), state);
    if(!state)
    {
        ui->loginCheckBox->setChecked(false);
        d->setDataFromIndex(LoginDlgPrivate::LoginCheckState, ui->idComboBox->currentIndex(), false);
    }
}

void LoginDlg::updateAutoLoginCheck(int index)
{
    Q_D(LoginDlg);

    QVariant state = d->getDataFromIndex(LoginDlgPrivate::LoginCheckState, index);
    ui->loginCheckBox->setChecked(state.toBool());
}

void LoginDlg::updateSavePWDCheck(int index)
{
    Q_D(LoginDlg);

    QVariant state = d->getDataFromIndex(LoginDlgPrivate::SaveCheckState, index);
    ui->saveCheckBox->setChecked(state.toBool());
}

void LoginDlg::updatePWD(int index)
{
    Q_D(LoginDlg);

    QVariant pwd = d->getDataFromIndex(LoginDlgPrivate::Password, index);
    ui->pwdLineEdit->setText(pwd.toString());
}
