#ifndef INPUTGRADEINFOWIZARDPAGE_H
#define INPUTGRADEINFOWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
    class InputGradeInfoWizardPage;
}

class InputGradeInfoWizardPagePrivate;

class InputGradeInfoWizardPage : public QWizardPage
{
    Q_OBJECT

    friend class InputGradeInfoWizardPagePrivate;

public:
    explicit InputGradeInfoWizardPage(QWidget *parent = 0);
    ~InputGradeInfoWizardPage();

    void initializePage();

private:
    Q_DISABLE_COPY(InputGradeInfoWizardPage)
    Ui::InputGradeInfoWizardPage * const ui;
    InputGradeInfoWizardPagePrivate * const d;
};

#endif // INPUTGRADEINFOWIZARDPAGE_H
