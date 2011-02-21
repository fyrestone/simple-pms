#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    task(Task::instance())
{
    ui->setupUi(this);
    connectSignalsAndSlots();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::connectSignalsAndSlots()
{
    connect(ui->inputSelectButton, SIGNAL(clicked()), this, SLOT(selectInputDB()));
    connect(ui->outputSelectButton, SIGNAL(clicked()), this, SLOT(selectOutputDB()));
}

void Widget::selectInputDB()
{
    QString input = QFileDialog::getOpenFileName(this,
                                                 tr("请选择旧版本数据库："),
                                                 QString(),
                                                 tr("PMS数据库文件 (*.db)"));

    ui->inputDBPath->setText(input);
}

void Widget::selectOutputDB()
{
    QString output = QFileDialog::getSaveFileName(this,
                                                  tr("请选择新版本数据库保存位置："),
                                                  QString(),
                                                  tr("PMS数据库文件 (*.db)"));

    ui->outputDBPath->setText(output);
}
