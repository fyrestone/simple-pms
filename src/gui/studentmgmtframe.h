#ifndef STUDENTMGMTFRAME_H
#define STUDENTMGMTFRAME_H

#include <QFrame>

namespace Ui {
    class StudentMgmtFrame;
}

class StudentMgmtFramePrivate;

class StudentMgmtFrame : public QFrame
{
    Q_OBJECT

    friend class StudentMgmtFramePrivate;

public:
    explicit StudentMgmtFrame(int gradeNum, int classNum, QWidget *parent = 0);
    ~StudentMgmtFrame();

private:
    Ui::StudentMgmtFrame * const ui;
    StudentMgmtFramePrivate * const d;
};

#endif // STUDENTMGMTFRAME_H
