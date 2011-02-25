#ifndef CLASSMGMTDLG_H
#define CLASSMGMTDLG_H

#include <QDialog>

namespace Ui {
    class ClassMgmtDlg;
}

class ClassMgmtDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ClassMgmtDlg(QWidget *parent = 0);
    ~ClassMgmtDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ClassMgmtDlg *ui;
};

#endif // CLASSMGMTDLG_H
