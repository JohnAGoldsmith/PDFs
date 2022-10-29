#include <QRegExp>
#include <QDebug>
#include "tok.h"
#include "widget.h"

QStringList break_up_regexp(QString & string){
    QRegExp regex1("(\\d )");
    QStringList output;
    int pos = 0;
    int offset = 0;
    while ((pos = regex1.indexIn(string, pos)) != -1) {
        output << regex1.cap(1).trimmed();
        pos += regex1.matchedLength();
        offset += regex1.matchedLength();
    }
    string = string.mid(offset);
    return output;
}
QString remove_prefix_and_remove_spaces(QString & string){
    QRegExp regex1("(\\d )");
    QString prefix;
    int pos = 0;
    int offset = 0;
    while ((pos = regex1.indexIn(string, pos)) != -1) {
        prefix +=  regex1.cap(1).trimmed();
        pos += regex1.matchedLength();
        offset += regex1.matchedLength();
    }
    string = string.mid(offset    );
    return prefix;
}


TreeItem::TreeItem(const QString prefix,
                   const QString data,
                   TreeItem *parent)
    : m_prefix(prefix), m_string(data), m_parentItem(parent)
{
    //qDebug() << 36 << "constructor" << m_key << "key length" << m_key.length() << "prefix" << m_prefix << m_string;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}


/* this is not used */
/*
bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QString data(columns);
        TreeItem *item = new TreeItem(QString(), QString::number(row), data, this);
        m_childItems.insert(position, item);
    }

    return true;
}
*/

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}
int TreeItem::row() const
{
    if (m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}
int TreeItem::columnCount() const
{   return 1;
}
QString TreeItem::data() const
{
    return m_prefix + " " + m_string;
}
int TreeItem::childNumber() const {
    if (m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}
bool TreeItem::setData( const QString &value)
{
    m_string  = value;
    return true;
}
TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}
void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}


ToK::ToK()
{

}



QList<Prefix_String*> ToK_model::output(bool key_only_flag){  // ignores root node.
    QList<Prefix_String*> list;
    foreach (TreeItem* item, *m_rootItem->get_childItems()){
        list = item->output(list, key_only_flag);
    }
    return list;
}
QList<Prefix_String*> TreeItem::output(QList<Prefix_String*> & list, bool key_only_flag){
    qDebug() << 99 << "new item"<<m_prefix << m_string;
    if (key_only_flag){
        Prefix_String * PS1 = new Prefix_String(QString(m_prefix.back()), m_string);
        list.append(PS1);
    } else{
        Prefix_String * PS2 = new Prefix_String(m_prefix, m_string);
        list.append(PS2);
    }
    foreach (TreeItem* item, m_childItems){
        QList<Prefix_String*> new_list;
        list.append(item->output(new_list, key_only_flag));
    }
    return list;
}
ToK_model::ToK_model(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(QString(), QString("Tree of knowledge"), nullptr);
    bool key_only_flag = false;
    output (key_only_flag);
}

ToK_model::~ToK_model()
{
    delete m_rootItem;
}
QModelIndex ToK_model::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid()){
        parentItem = m_rootItem;
    }
    else{
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    TreeItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}
QModelIndex ToK_model::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ToK_model::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ToK_model::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return m_rootItem->columnCount();
}


QVariant ToK_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    //return item->data();
    return item->get_prefix() + " " + item->get_string();
}
Qt::ItemFlags ToK_model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant ToK_model::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data();

    return QVariant();
}
void ToK_model::addItem(  QString & prefix,  const QString & string){
    if (prefix.length() == 0) {return;}
    QString prefix_copy = prefix;
    //qDebug() << "";
    //qDebug() << string;
    TreeItem * item = m_rootItem->find_place_in_tree(prefix_copy);
    item->setData(string);
    item->set_prefix(prefix);
    //qDebug() << 239 << "prefix" << item->get_prefix() << "length" << prefix.length() << item->get_string();
    //qDebug() << 239 << "End of adding item"<< "\n";
}

TreeItem* TreeItem::find_child_ending_with_char(QChar this_char){
    for (int i = 0; i < m_childItems.size(); i++){
        if (m_childItems[i]->get_prefix().back() == this_char ){
            return m_childItems[i];
        }
    }
    return nullptr;
}

int TreeItem::find_proper_location_among_children(QChar this_char){
    for (int i = 0; i < m_childItems.size(); i++){
        if ( m_childItems.at(i)->get_prefix().back() > this_char){
            return i;
        }
    }
    return -1;
}
TreeItem * TreeItem::find_place_in_tree(QString & prefix) {
    QChar key = prefix[0];
    prefix = prefix.mid(1);
    //qDebug() << 244 << "->"<< "current node prefix and string :"  << m_prefix << m_string <<"key for new item" <<key <<  "prefix after key"<<prefix;
    TreeItem * right_child = find_child_ending_with_char(key);
    if (right_child){
        switch (prefix.length()){
            case 0:{
                /* this should not happen */
                return right_child;
            }
            case 1:{
                /* this item is terminal and sits right under right_child */
                QString new_prefix = right_child->get_prefix() + key;
                TreeItem * item = new TreeItem(new_prefix, QString(), right_child);
                right_child->get_childItems()->append(item);
                return item;
            }
            default:{
                /*  we must go deeper into tree */
                return right_child->find_place_in_tree(prefix);
            }
        }
    } else{
        TreeItem* item = new TreeItem(m_prefix + key, QString(), this);
        int n = find_proper_location_among_children(key);
        switch (prefix.length()){
            case 0:{
                if (find_proper_location_among_children(key) >= 0 ){
                    m_childItems.insert(n,item);
                    return item;
                } else{
                    m_childItems.append(item);
                    return item;
                }
            }
            default:{
                if (find_proper_location_among_children(key) >= 0){
                    m_childItems.insert(n, item);
                    return item->find_place_in_tree(prefix);
                } else{
                    m_childItems.append(item);
                    return item->find_place_in_tree(prefix);
                }

            }
        }
     }
}


