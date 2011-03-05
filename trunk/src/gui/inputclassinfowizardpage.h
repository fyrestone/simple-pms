#ifndef INPUTCLASSINFOWIZARDPAGE_H
#define INPUTCLASSINFOWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
    class InputClassInfoWizardPage;
}

class InputClassInfoWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit InputClassInfoWizardPage(QWidget *parent = 0);
    ~InputClassInfoWizardPage();

private:
    Ui::InputClassInfoWizardPage *ui;
};

#endif // INPUTCLASSINFOWIZARDPAGE_H
