#ifndef SELECTGRADEWIZARDPAGE_H
#define SELECTGRADEWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
    class SelectGradeWizardPage;
}

class SelectGradeWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectGradeWizardPage(QWidget *parent = 0);
    ~SelectGradeWizardPage();

private:
    Ui::SelectGradeWizardPage *ui;
};

#endif // SELECTGRADEWIZARDPAGE_H
