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

public slots:
    void initializeAndShow();

protected:
    void changeEvent(QEvent *e);

private:
    void initializeAll();
    void setCustomApperance();
    void connectSignalsAndSlots();

private slots:
    void testSlot();

private:
    Q_DISABLE_COPY(MainWin)
    Ui::MainWin * const ui;
    MainWinPrivate * const d;
};

#endif // MAINWIN_H
