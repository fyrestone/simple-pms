#ifndef CLASSMGMTDLG_P_H
#define CLASSMGMTDLG_P_H

#include <QtCore/QObject>               /* 使用Q_DECLARE_PUBLIC */
#include "../engine/dataengine.h"       /* 使用Task */

class ClassMgmtDlg;

class ClassMgmtDlgPrivate : public QObject
{
    Q_OBJECT

public:
    ClassMgmtDlgPrivate(ClassMgmtDlg *parent);

public:
    DataEngine::Task *task;

private slots:
    void finished(int taskID, const QVariant &result);

private:
    Q_DISABLE_COPY(ClassMgmtDlgPrivate)
    ClassMgmtDlg * const q;
};

#endif // CLASSMGMTDLG_P_H
