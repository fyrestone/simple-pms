#ifndef INPUTGRADEINFOWIZARDPAGE_P_H
#define INPUTGRADEINFOWIZARDPAGE_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */

class InputGradeInfoWizardPage;

class InputGradeInfoWizardPagePrivate : public QObject
{
    Q_OBJECT

public:
    InputGradeInfoWizardPagePrivate(InputGradeInfoWizardPage *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();
    bool validatePage();

private:
    Q_DISABLE_COPY(InputGradeInfoWizardPagePrivate)
    DataEngine::Task * const task;
    InputGradeInfoWizardPage * const q;
};

#endif // INPUTGRADEINFOWIZARDPAGE_P_H
