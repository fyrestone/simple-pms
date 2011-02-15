#include <QtGui/QApplication>
#include "../../src/gui/tabpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TabPage w;
    w.show();

    return a.exec();
}
