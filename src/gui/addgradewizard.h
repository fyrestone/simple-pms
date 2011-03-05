#ifndef ADDGRADEWIZARD_H
#define ADDGRADEWIZARD_H

#include <QWizard>

namespace Ui {
    class AddGradeWizard;
}

class AddGradeWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AddGradeWizard(QWidget *parent = 0);
    ~AddGradeWizard();

private:
    Ui::AddGradeWizard *ui;
};

#endif // ADDGRADEWIZARD_H
