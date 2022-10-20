#ifndef TOK_H
#define TOK_H

#include <QVariant>
#include <QAbstractItemModel>

class TreeItem;

class ToK
{
public:
    ToK();
};
class ToK_model : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ToK_model(const QString &data, QObject *parent = nullptr);
    explicit ToK_model(QObject *parent);
    ~ToK_model();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);
    void addItem(const QString  & data);

    TreeItem *m_rootItem;
};
class TreeItem
{
public:
    explicit TreeItem(const QString key, const QString &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);


    TreeItem *  child(int row);
    int         childCount() const;
    int         columnCount() const;
    bool        insertChildren(int position, int count, int columns);
    QString     data( ) const;
    int         row() const;
    TreeItem *  parentItem();
    bool        removeChildren(int position, int count);
    bool        removeColumns(int position, int columns);
    int         childNumber() const;
    bool        setData(const QString &value);
    TreeItem *  find_place_in_tree(QStringList & prefix);
    QString     get_key() {return m_key;}
    QString     get_string() {return m_string;}


private:
    QList<TreeItem *>   m_childItems;
    QString             m_string;
    QString             m_key;  //this QString is a digit, marking this item's particular key, which appears inside the prefix of m_itemData
    //QList<QVariant>   m_itemData;
    TreeItem *          m_parentItem;
    int                 m_depth; // root is depth 0; its children are depth 1.

};

#endif // TOK_H