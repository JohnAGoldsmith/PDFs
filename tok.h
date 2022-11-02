#ifndef TOK_H
#define TOK_H

#include <QVariant>
#include <QAbstractItemModel>

class TreeItem;
struct Prefix_String;

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
    QList<Prefix_String*>         output(bool key_only_flag); // if key_only_flag is true, then the prefix is just the key, not the entire prefix. Key only is preferred for tableview
                                                    // the whole prefix is preferred for output to json file.
    //void addItem(const QString  & data);
    void addItem(   QString & prefix, const QString & data);

private:


    TreeItem *m_rootItem;
};
class TreeItem
{
public:
    explicit TreeItem(QString prefix, QString data , TreeItem *parentItem );
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
    QList<TreeItem*> * get_childItems() {return &m_childItems;}
    bool        setData(const QString &value);
    TreeItem *  find_place_in_tree(QString & prefix);
    //QString     get_key() {return m_key;}
    QString     get_string() {return m_string;}
    QList<Prefix_String*> output(QList<Prefix_String*>&, bool key_only_flag);
    QString     get_prefix() {return m_prefix;}
    void        set_prefix(QString prefix) {m_prefix = prefix;}


private:
    QList<TreeItem *>   m_childItems;
    QString             m_string;
    QString             m_prefix; // the string of digits and spaces; the "key" is the last digit in the prefix
    //QString           m_key;  //this QString is a digit, marking this item's particular key, which appears inside the prefix of m_itemData
    //QList<QVariant>   m_itemData;
    TreeItem *          m_parentItem;
    //int               m_depth; // root is depth 0; its children are depth 1.
    TreeItem*           find_child_ending_with_char(QChar);
    int                 find_proper_location_among_children(QChar );

};

#endif // TOK_H
