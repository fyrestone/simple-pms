#include "gui/logindlg.h"
#include "gui/mainwin.h"

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QPlastiqueStyle>
#include <QThread>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 设置中文支持 */
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    
    /* 加载Qt中文字符 */
    QTranslator translator;
    if(translator.load("zh_CN", QObject::tr(":/lang")))
        a.installTranslator(&translator);

    /* 设置应用程序风格 */
    QApplication::setStyle(new QPlastiqueStyle);//设置为PlastiqueStyle
    //QApplication::setStyle(new QCleanlooksStyle);//设置为PlastiqueStyle

    LoginDlg w; //先构造登陆对话框
    MainWin m;  //再构造登陆对话框

    QObject::connect(&w, SIGNAL(accepted()), &m, SLOT(show()));

    w.show();

    qDebug() << QThread::currentThreadId() << "MainThread";

    return a.exec();
}
