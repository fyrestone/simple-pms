#ifndef MAINWIN_P_H
#define MAINWIN_P_H

#include "aboutdlg.h"               /* 使用AboutDlg */
#include "classmgmtdlg.h"           /* 使用ClassMgmtDlg */
#include "addgradewizard.h"         /* 使用AddGradeWizard */
#include "addclasswizard.h"         /* 使用AddClassWizard */
#include "../engine/dataengine.h"   /* 使用Task */

#include <QtCore/QObject>           /* 使用QObject */
#include <QPushButton>              /* addTabButton/delTabButton */
#include <QMenu>

class MainWin;

class MainWinPrivate : public QObject
{
    Q_OBJECT

public:
    MainWinPrivate(MainWin *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

private slots:
    void finished(int taskID, const QVariant &result);
    void showNavigationContextMenu(const QPoint &pos);
    void showAddGradeWizard();
    void showAddClassWizard();
    void showClassMgmtDlg();
    void deleteGradeClass();

private:
    Q_DISABLE_COPY(MainWinPrivate)
    DataEngine::Task * const task;
    MainWin * const q;
    AboutDlg aboutDlg;
    QPushButton addTabButton;
    QPushButton delTabButton;
    QMenu rootContextMenu;
    QMenu gradeContextMenu;
    QMenu classContextMenu;
};

#endif // MAINWIN_P_H
