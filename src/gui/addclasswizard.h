#ifndef ADDCLASSWIZARD_H
#define ADDCLASSWIZARD_H

#include <QWizard>

namespace Ui {
    class AddClassWizard;
}

class AddClassWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AddClassWizard(QWidget *parent = 0);
    ~AddClassWizard();

private:
    Ui::AddClassWizard *ui;
};

#endif // ADDCLASSWIZARD_H
