#ifndef NAVIGATIONITEM_H
#define NAVIGATIONITEM_H

#include <QTreeWidgetItem>

class NavigationItem : public QTreeWidgetItem
{
public:
    enum Type                           ///  导航树节点类型枚举
    {
        /* QTreeWidgetItem::ItemType的1000以下Qt保留使用 */
        Root = 1001,                    ///< 根节点
        Grade = 1002,                   ///< 年级节点
        Class = 1003                    ///< 班级节点
    };

    enum Role                           ///  节点角色
    {
        ExtraData = Qt::UserRole + 1    ///< 用于存取额外数据
    };

    NavigationItem(int type);
    NavigationItem(QTreeWidget *parent, int type);
    NavigationItem(QTreeWidgetItem *parent, int type);
    NavigationItem(const NavigationItem &other);
    NavigationItem &operator=(const NavigationItem &other);

    QVariant data(int column, int role) const;
    void setData(int column, int role, const QVariant &value);

private:
    QVariant extraData;
};

#endif // NAVIGATIONITEM_H
