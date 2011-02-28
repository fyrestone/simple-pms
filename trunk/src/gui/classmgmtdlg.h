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
    explicit ClassMgmtDlg(QWidget *parent = 0);
    ~ClassMgmtDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Q_DISABLE_COPY(ClassMgmtDlg)
    Ui::ClassMgmtDlg * const ui;
    ClassMgmtDlgPrivate * const d;
};

#endif // CLASSMGMTDLG_H
