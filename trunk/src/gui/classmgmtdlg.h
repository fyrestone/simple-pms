#ifndef CLASSMGMTDLG_H
#define CLASSMGMTDLG_H

#include <QDialog>

namespace Ui {
    class ClassMgmtDlg;
}

class ClassMgmtDlgPrivate;

class ClassMgmtDlg : public QDialog
{
    Q_OBJECT

    friend class ClassMgmtDlgPrivate;

public:
    explicit ClassMgmtDlg(int gradeNum, int classNum, QWidget *parent = 0);
    ~ClassMgmtDlg();

private:
    Ui::ClassMgmtDlg * const ui;
    ClassMgmtDlgPrivate * const d;
};

#endif // CLASSMGMTDLG_H
