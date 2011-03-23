#ifndef INPUTCLASSINFOWIZARDPAGE_P_H
#define INPUTCLASSINFOWIZARDPAGE_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */
#include <QStandardItemModel>           /* 使用QStandardItemModel */

class InputClassInfoWizardPage;

class InputClassInfoWizardPagePrivate : public QObject
{
    Q_OBJECT

public:
    InputClassInfoWizardPagePrivate(InputClassInfoWizardPage *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();
    bool validatePage();

private:
    Q_DISABLE_COPY(InputClassInfoWizardPagePrivate)
    DataEngine::Task * const task;
    InputClassInfoWizardPage * const q;
    QStandardItemModel model;
};

#endif // INPUTCLASSINFOWIZARDPAGE_P_H
