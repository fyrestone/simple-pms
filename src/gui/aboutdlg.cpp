#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);

    ui->compileLabel->setText(__DATE__ " " __TIME__);
    ui->aboutTextEdit->setText(tr(
            "&nbsp;&nbsp;<b>学生成绩管理系统</b>由<b>刘宝</b>专为<b>河南省驻马店市第一初级中学</b>定制。<br/>"
            "&nbsp;&nbsp;感谢软件开发期间<b>老爸（刘超）</b>和<b>老妈（王玉云）</b>对我工作的支持！<br/>"
            "&nbsp;&nbsp;软件难免有Bug，如果您在使用中遇到了程序崩溃或者运行异常，请点击<b>右下角</b>的<b>技术支持</b>链接！<br/>"
            "&nbsp;&nbsp;本软件所使用的Qt框架库版权归Nokia公司所有，LZMA SDK版权归Igor Pavlov所有。"));
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
