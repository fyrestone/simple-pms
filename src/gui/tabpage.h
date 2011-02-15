#ifndef TABPAGE_H
#define TABPAGE_H

#include <QFrame>

namespace Ui {
    class TabPage;
}

class TabPagePrivate;

class TabPage : public QFrame
{
    Q_OBJECT

public:
    explicit TabPage(QWidget *parent = 0);
    ~TabPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TabPage *ui;
    TabPagePrivate * const d_ptr;
};

#endif // TABPAGE_H
