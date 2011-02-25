#ifndef MAINWIN_P_H
#define MAINWIN_P_H

#include <QtCore/QObject>           /* 使用QObject */
#include <QPushButton>              /* addTabButton/delTabButton */
#include "aboutdlg.h"               /* 使用AboutDlg */
#include "../engine/dataengine.h"   /* 使用Task */

class MainWinPrivate : public QObject
{
    Q_OBJECT

public:
    MainWinPrivate();

public:
    DataEngine::Task *task;
    AboutDlg aboutDlg;
    QPushButton addTabButton;
    QPushButton delTabButton;

signals:
    void expandClassTree();

private slots:
    void finished(int taskID, const QVariant &result);

private:
    Q_DISABLE_COPY(MainWinPrivate)
};

#endif // MAINWIN_P_H
