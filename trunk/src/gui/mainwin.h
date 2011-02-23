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

public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();

protected:
    void changeEvent(QEvent *e);

private:
    void setCustomApperance();
    void connectSignalsAndSlots();

private:
    Q_DISABLE_COPY(MainWin)
    Ui::MainWin * const ui;
    MainWinPrivate * const d;
};

#endif // MAINWIN_H
