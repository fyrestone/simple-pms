#ifndef MAINWIN_P_H
#define MAINWIN_P_H

#include <QtCore/QObject>   /* Q_DECLARE_PUBLIC使用 */
#include <QPushButton>      /* addTabButton/delTabButton */
#include "aboutdlg.h"       /* AboutDlg使用 */

class MainWinPrivate
{
    Q_DECLARE_PUBLIC(MainWin)

public:
    MainWinPrivate(MainWin *parent);

private:
    MainWin * const q_ptr;
    AboutDlg aboutDlg;
    QPushButton addTabButton;
    QPushButton delTabButton;
};

MainWinPrivate::MainWinPrivate(MainWin *parent)
    :q_ptr(parent)
{
    QIcon addTabIcon, delTabIcon;

    addTabIcon.addFile(QString(QObject::tr(":/Icon/image/add.png")), QSize(), QIcon::Normal, QIcon::Off);
    addTabButton.setIcon(addTabIcon);
    addTabButton.setFlat(true);

    delTabIcon.addFile(QString(QObject::tr(":/Icon/image/delete.png")), QSize(), QIcon::Normal, QIcon::Off);
    delTabButton.setIcon(delTabIcon);
    delTabButton.setFlat(true);
}

#endif // MAINWIN_P_H
