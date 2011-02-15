#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>

namespace Ui {
    class MainWin;
}

class MainWinPrivate;

class MainWin : public QMainWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MainWin)

public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();

protected:
    void changeEvent(QEvent *e);

private:
    void setCustomApperance();
    void connectSignalsAndSlots();

private:
    Ui::MainWin *ui;
    MainWinPrivate * const d_ptr;
};

#endif // MAINWIN_H
