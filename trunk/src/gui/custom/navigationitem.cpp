#include "navigationitem.h"

NavigationItem::NavigationItem(int type)
    :QTreeWidgetItem(type)
{
}

NavigationItem::NavigationItem(QTreeWidget *parent, int type)
    :QTreeWidgetItem(parent, type)
{
}

NavigationItem::NavigationItem(QTreeWidgetItem *parent, int type)
    :QTreeWidgetItem(parent, type)
{
}

NavigationItem::NavigationItem(const NavigationItem &other)
    :QTreeWidgetItem(other), extraData(other.extraData)
{
}

NavigationItem &NavigationItem::operator =(const NavigationItem &other)
{
    QTreeWidgetItem::operator =(other);
    extraData = other.extraData;
    return *this;
}

QVariant NavigationItem::data(int column, int role) const
{
    if(role == ExtraData)
        return extraData;
    else
        return QTreeWidgetItem::data(column, role);
}

void NavigationItem::setData(int column, int role, const QVariant &value)
{
    if(role == ExtraData)
        extraData = value;
    else
        QTreeWidgetItem::setData(column, role, value);
}
