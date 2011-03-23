#ifndef SELECTGRADEWIZARDPAGE_P_H
#define SELECTGRADEWIZARDPAGE_P_H

#include "../engine/dataengine.h"       /* 使用Task */

#include <QtCore/QObject>               /* 继承自QObject */

class SelectGradeWizardPage;
class QTreeWidgetItem;

class SelectGradeWizardPagePrivate : public QObject
{
    Q_OBJECT

public:
    SelectGradeWizardPagePrivate(SelectGradeWizardPage *parent);

    void initializeMember();
    void connectSignalsAndSlots();
    void completeConstruct();
    bool validatePage();

private slots:
    void doItemClicked(QTreeWidgetItem *item, int column);

private:
    Q_DISABLE_COPY(SelectGradeWizardPagePrivate)
    DataEngine::Task * const task;
    SelectGradeWizardPage * const q;
};

#endif // SELECTGRADEWIZARDPAGE_P_H
