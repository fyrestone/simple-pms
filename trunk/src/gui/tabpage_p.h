#ifndef TABPAGE_P_H
#define TABPAGE_P_H

#include <QtCore/QObject>   /* Q_DECLARE_PUBLIC使用 */

class TabPagePrivate
{
    Q_DECLARE_PUBLIC(TabPage)

public:
    TabPagePrivate(TabPage *parent) : q_ptr(parent){}

private:
    TabPage * const q_ptr;
};

#endif // TABPAGE_P_H
