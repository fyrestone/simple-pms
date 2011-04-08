#ifndef CLASSMGMTDLG_P_H
#define CLASSMGMTDLG_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */

class ClassMgmtDlg;

class ClassMgmtDlgPrivate : public QObject
{
    Q_OBJECT

public:
    ClassMgmtDlgPrivate(ClassMgmtDlg *parent, int gradeNum, int classNum);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

private:
    DataEngine::Task * const task;
    ClassMgmtDlg * const q;
    const int gradeNum;
    const int classNum;
};

#endif // CLASSMGMTDLG_P_H
