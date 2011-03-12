#ifndef INPUTCLASSINFOWIZARDPAGE_P_H
#define INPUTCLASSINFOWIZARDPAGE_P_H

#include <QtCore/QObject>               /* 继承自QObject */
#include "../engine/dataengine.h"       /* 使用Task */

class InputClassInfoWizardPage;

class InputClassInfoWizardPagePrivate : public QObject
{
    Q_OBJECT

public:
    InputClassInfoWizardPagePrivate(InputClassInfoWizardPage *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();

    bool isComplete() const;
    bool validatePage();

private slots:
    void finished(int taskID, const QVariant &result);

private:
    Q_DISABLE_COPY(InputClassInfoWizardPagePrivate)
    DataEngine::Task * const task;
    InputClassInfoWizardPage * const q;
    bool hasTaskRunning;
};

#endif // INPUTCLASSINFOWIZARDPAGE_P_H
