#include <QRegExp>
#include <QDebug>
#include "tok.h"

QStringList break_up_regexp(QString & string){
    QRegExp regex1("(\\d )");
    QStringList output;
    int pos = 0;
    int offset = 0;
    while ((pos = regex1.indexIn(string, pos)) != -1) {
        output << regex1.cap(1);
        pos += regex1.matchedLength();
        offset += regex1.matchedLength();
    }
    string = string.mid(offset);
    return output;
}
QString cut_off_prefix(QString & string){
    QRegExp regex1("(\\d )");
    QString prefix;
    int pos = 0;
    int offset = 0;
    while ((pos = regex1.indexIn(string, pos)) != -1) {
        prefix +=  regex1.cap(1) ;
        pos += regex1.matchedLength();
        offset += regex1.matchedLength();
    }
    string = string.mid(offset    );
    return prefix;
}


TreeItem::TreeItem(const QString prefix, const QString key, const QString &data, TreeItem *parent)
    : m_prefix(prefix.trimmed()), m_key(key), m_string(data), m_parentItem(parent)
{
    qDebug() << "constructor" << m_key << "key length" << m_key.length() << "prefix" << m_prefix << m_prefix.length();
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}
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
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
int TreeItem::columnCount() const
{   return 1;
    //return m_itemData.count();
}
QString TreeItem::data() const
{
    return m_key + m_string;
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
QStringList ToK_model::output(bool key_only_flag){
    QStringList result;
    result = m_rootItem->output(result, key_only_flag);
    qDebug() << 96 <<"End result:" << result;
    return result;
}
QStringList TreeItem::output(QStringList& list, bool key_only_flag){
    qDebug() << 99 << "new item"<<m_key << m_string;
    if (key_only_flag){
        list.append(m_key + " " + m_string);
    } else{
        list.append(m_prefix + " " + m_string);
    }
    foreach (TreeItem* item, m_childItems){
        qDebug() << 101 << item->data();
        QStringList new_list;
        //list.append(item->get_key() + ":" + item->get_string() + "(103)");
        list.append(item->output(new_list, key_only_flag));
    }

    //qDebug() << "\n" <<105 <<  list;
    return list;
}


ToK_model::ToK_model(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(QString(), QString::number(0), tr("Tree of knowledge"));



    setupModelData(data.split('\n'), m_rootItem);
}
ToK_model::ToK_model(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(QString::number(0), QString::number(0), tr("Tree of knowledge"));

    /*
    addItem("0 1 Zellig to Noam");
    addItem("0 2 lxa and segmentation");
    addItem("0 3 prime suspect");
    addItem("0 3 1 Sidney Reilly");
    addItem("0 3 2 Gurdjieff");
    addItem("0 3 3 prime numbers");
    addItem("0 4 Battle in the mind field");
    addItem("0 4 1 chapter 1");
    addItem("0 4 2 chapter 2");
    addItem("0 4 2 1 IndoEuropean");
    addItem("0 4 3 chapter 3");
    addItem("0 5 JFK");
    addItem("0 6 Confrontation (Volume 2)");
    addItem("1 Linguistics");
    addItem("1 1 Phonology");
    addItem("1 2 Morphology");
    */


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

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
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
    return item->get_key() + " " + item->get_string();
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
void ToK_model::addItem(const QString & string){
    QString line(string);
    QStringList prefix = break_up_regexp(line);//line is modified by this function; its prefix is removed.
    if (prefix.length() == 0) {return;}
    qDebug() << 257 << "prefix" << prefix << "length" << prefix.length();
    TreeItem * item = m_rootItem->find_place_in_tree(prefix);
    item->setData(line);
}
/*
void ToK_model::addItem(const QString & prefix, const QString & string){
    TreeItem * item = m_rootItem->find_place_in_tree(prefix.split(" "));
    item->setData(string);
}
*/
TreeItem * TreeItem::find_place_in_tree(QStringList& prefixes) {
    QString first = prefixes.takeFirst().trimmed();
    qDebug() << 260 << ">" << first << "<"<< "length" << first.length();
    for (int i = 0; i < m_childItems.count(); i++){
        if (m_childItems[i]->get_key() == first){
            if (prefixes.size() == 0){
                return m_childItems[i];
            }
            return m_childItems[i]->find_place_in_tree(prefixes);
        }
        if (m_childItems[i]->get_key() > first){
            QString prefix = m_prefix  + first; // ***

            TreeItem * item = new TreeItem(prefix, first, QString(), this);
            m_childItems.insert(i, item);
            if (prefixes.size()== 0){
                return item;
            }
            return item->find_place_in_tree(prefixes);
        }
    }
    QString prefix = m_prefix + " " + first;
    qDebug() << 269 << prefix; // good
    TreeItem * item = new TreeItem(prefix, first, QString(), this);
    m_childItems.append(item);
    if (prefixes.size() == 0){
        return item;
    }
    return item->find_place_in_tree(prefixes);
}
void ToK_model::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem *> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
            QList<QVariant> columnData;
            columnData.reserve(columnStrings.count());
            QString this_string;
            for (const QString &columnString : columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(QString::number(0),QString::number(0), this_string, parents.last()));
        }
        ++number;
    }
}



