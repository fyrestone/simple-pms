#ifndef INPUTCLASSINFOWIZARDPAGE_H
#define INPUTCLASSINFOWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
    class InputClassInfoWizardPage;
}

class InputClassInfoWizardPagePrivate;

class InputClassInfoWizardPage : public QWizardPage
{
    Q_OBJECT

    friend class InputClassInfoWizardPagePrivate;

public:
    explicit InputClassInfoWizardPage(QWidget *parent = 0);
    ~InputClassInfoWizardPage();

    void initializePage();
    bool isComplete() const;
    bool validatePage();

private:
    Q_DISABLE_COPY(InputClassInfoWizardPage)
    Ui::InputClassInfoWizardPage * const ui;
    InputClassInfoWizardPagePrivate * const d;
};

#endif // INPUTCLASSINFOWIZARDPAGE_H
