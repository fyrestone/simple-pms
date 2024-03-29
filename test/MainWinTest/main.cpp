#include <QtGui>
#include "../../src/gui/mainwin.h"


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

    MainWin w;
    w.show();

    return a.exec();
}
