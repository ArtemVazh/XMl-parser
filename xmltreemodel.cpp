#include "treeitem.h"
#include "XmlTreeModel.h"
#include "xmlparser.h"
#include "ui_xmlparser.h"

XmlTreeModel::XmlTreeModel(const QList<QString> &fileName, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_fileName = fileName;
    QList<QVariant> rootData;
    rootData << "";
    rootItem = new TreeItem(rootData);
    parents << rootItem;
}

XmlTreeModel::~XmlTreeModel()
{
    delete rootItem;
}

int XmlTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant XmlTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags XmlTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant XmlTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex XmlTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex XmlTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int XmlTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

bool XmlTreeModel::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes) {
    QList<QVariant> data;
    if(qName == "array")
        alreadyChanged = false;
    if(qName == "catalog") {
        alreadyChanged = false;
        QString name = QString::fromStdString(m_fileName[count].toStdString().substr(m_fileName[count].toStdString().find_last_of("/") + 1, m_fileName[count].toStdString().size()));
        data << QVariant(name);
        count++;
    }
    else
        data << QVariant(qName);
    parents.last()->appendChild(new TreeItem(data, parents.last()));
    parents << parents.last()->child(parents.last()->childCount() - 1);
    if (qName.toStdString().substr(0,4) == "Name" || qName == "CommonName"){
        isName = true;
    }
    return true;
}

bool XmlTreeModel::endElement(const QString &namespaceURI, const QString &localName, const QString &qName){
    parents.pop_back();
    return true;
}

bool XmlTreeModel::fatalError(const QXmlParseException &exception){
    QMessageBox::critical(nullptr, "Ошибка", exception.message(), QMessageBox::Ok);
    return true;
}

bool XmlTreeModel::characters(const QString &str){
    if(!str.contains(QRegExp("\\S")))
        return true;
    if(isName == true && alreadyChanged==false){
        QList<QVariant> data;
        data << QVariant(str);
        parents[2]->appendData(data);
        isName = false;
        alreadyChanged=true;
    }
    QList<QVariant> data;
    data << QVariant(str);
    parents.last()->appendChild(new TreeItem(data, parents.last()));
    return true;
}
