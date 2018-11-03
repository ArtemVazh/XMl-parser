#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QMainWindow>
#include <QtWidgets>
#include <QtXml>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();
    void appendChild(TreeItem *child);
    void appendData(const QList<QVariant> &data);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
