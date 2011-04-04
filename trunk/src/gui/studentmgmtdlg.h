#ifndef STUDENTMGMTDLG_H
#define STUDENTMGMTDLG_H

#include <QDialog>

namespace Ui {
    class StudentMgmtDlg;
}

class StudentMgmtDlgPrivate;

class StudentMgmtDlg : public QDialog
{
    Q_OBJECT

    friend class StudentMgmtDlgPrivate;

public:
    explicit StudentMgmtDlg(QWidget *parent = 0);
    ~StudentMgmtDlg();

private:
    Ui::StudentMgmtDlg * const ui;
    StudentMgmtDlgPrivate * const d;
};

#endif // STUDENTMGMTDLG_H
