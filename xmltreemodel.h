#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtXml>
#include <QtWidgets>
#include <QStringList>


class TreeItem;

class XmlTreeModel : public QAbstractItemModel, public QXmlDefaultHandler
{
    Q_OBJECT

public:
    explicit XmlTreeModel(const QList<QString> &fileName, QObject *parent = 0);
    ~XmlTreeModel();
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
private:
    int count = 0;
    QList<QString> m_fileName;
//    QList<QString*> m_name;
    bool isName = false;
    bool alreadyChanged = false;
    TreeItem *rootItem;
    QList<TreeItem*> parents;
};

#endif // XmlTreeModel_H
