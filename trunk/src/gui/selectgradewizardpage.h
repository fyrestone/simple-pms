#ifndef SELECTGRADEWIZARDPAGE_H
#define SELECTGRADEWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
    class SelectGradeWizardPage;
}

class SelectGradeWizardPagePrivate;

class SelectGradeWizardPage : public QWizardPage
{
    Q_OBJECT

    friend class SelectGradeWizardPagePrivate;

public:
    explicit SelectGradeWizardPage(QWidget *parent = 0);
    ~SelectGradeWizardPage();

    void initializePage();
    bool isComplete() const;
    bool validatePage();

private:
    Q_DISABLE_COPY(SelectGradeWizardPage)
    Ui::SelectGradeWizardPage * const ui;
    SelectGradeWizardPagePrivate * const d;
};

#endif // SELECTGRADEWIZARDPAGE_H
