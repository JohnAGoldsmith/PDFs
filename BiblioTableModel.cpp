#include "widget.h"
#include <QGridLayout>
#include <QFileSystemModel>
#include <QSettings>
#include <QJsonParseError>
#include <QFileDialog>
#include <QJsonArray>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDesktopServices>
#include <QStandardItem>
#include <QComboBox>
#include <QDropEvent>
#include <QMap>
#include <QShortcut>
#include <QSplitter>
#include <QMessageBox>
#include <QList>
#include <QSortFilterProxyModel>
#include <QScrollBar>
#include<QAbstractScrollArea>
#include <EGL/egl.h>
class List;


BiblioTableModel::BiblioTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}
BiblioTableModel::BiblioTableModel(QList< Entry* > entrylist, QObject *parent)
    : QAbstractTableModel(parent){

}

void BiblioTableModel::register_all_entries()
{
     foreach (Entry* entry, m_entries){
         register_biblioentry_by_key(entry);
         register_biblioentry_by_size(entry);
         register_biblioentry_by_filenamestem(entry);
         register_biblioentry_by_fullfilename(entry);
     }
}
/*
void BiblioTableModel::register_entry(Entry * entry){
    register_biblioentry_by_key(entry);
    register_biblioentry_by_filenamestem(entry);
    register_biblioentry_by_fullfilename(entry);
    register_biblioentry_by_size(entry);
}
*/
void BiblioTableModel::register_biblioentry_by_key(Entry* entry){
    if (m_data_by_key_multiple.contains(entry->get_key())){
        m_data_by_key_multiple.insert(entry->get_key(), entry);
    } else {
        if (m_data_by_key.contains(entry->get_key() ) ) {
              m_data_by_key_multiple.insert(entry->get_key(), m_data_by_filenamestem[entry->get_filenamestem()]);
              m_data_by_key_multiple.insert(entry->get_key(), entry);
            }
           else {
            m_data_by_key.insert(entry->get_key(), entry);
           }
    }

}
void BiblioTableModel::register_biblioentry_by_size(Entry * entry ){
    if (m_data_by_size_multiple.contains(entry->get_size())){
        m_data_by_size_multiple.insert(entry->get_size(), entry);
    } else {
        if (m_data_by_size.contains(entry->get_size() ) ) {
              m_data_by_size_multiple.insert(entry->get_size(), m_data_by_size[entry->get_size()]);
              m_data_by_size_multiple.insert(entry->get_size(), entry);
            }
           else {
              m_data_by_size.insert(entry->get_size(), entry);
           }
    }
}
void BiblioTableModel::register_biblioentry_by_fullfilename(Entry* entry){
    if (m_data_by_fullfilename_multiple.contains(entry->get_filenamefull())){
        m_data_by_fullfilename_multiple.insert(entry->get_filenamefull(), entry);
    } else {
        if (m_data_by_fullfilename_multiple.contains(entry->get_filenamefull() ) ) {
              m_data_by_fullfilename_multiple.insert(entry->get_filenamefull(), m_data_by_fullfilename[entry->get_filenamefull()]);
              m_data_by_fullfilename_multiple.insert(entry->get_filenamefull(), entry);
            }
         else {
              m_data_by_fullfilename.insert(entry->get_filenamefull(), entry);
         }
    }
}
void BiblioTableModel::register_biblioentry_by_filenamestem(Entry* entry){
    if (m_data_by_filenamestem_multiple.contains(entry->get_filenamestem())){
        m_data_by_filenamestem_multiple.insert(entry->get_filenamestem(), entry);
    } else {
        if (m_data_by_filenamestem.contains(entry->get_filenamestem() ) ) {
              m_data_by_filenamestem_multiple.insert(entry->get_filenamestem(), m_data_by_filenamestem[entry->get_filenamestem()]);
              m_data_by_filenamestem_multiple.insert(entry->get_filenamestem(), entry);
            }
         else {
              m_data_by_filenamestem.insert(entry->get_filenamestem(), entry);
         }
    }
}
QList<Entry*> BiblioTableModel::contains_filenamestem(QString stem){
    if (m_data_by_filenamestem.contains(stem)){
        return m_data_by_filenamestem.values(stem);
    } else{
        return QList<Entry*>();
    }
}

void BiblioTableModel::add_entry(Entry *entry){
    m_entries.append(entry);


}
int BiblioTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_entries.size();
}
int BiblioTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 12;
}
void BiblioTableModel::replace_entry(int row, Entry * entry){
    if (row < 0 || row >= m_entries.count()){
        qDebug() << 48 << "Attempted to add an item to an impossible row number "<< row;
        return;
    }
    m_entries[row] = entry;
}
QVariant BiblioTableModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();
     if (index.row() >= m_entries.size() || index.row() < 0)
         return QVariant();
     Entry* entry   = m_entries.at(index.row());


     if (role == Qt::DisplayRole) {
         switch (index.column()){
            case 0:
                 return invert_first_author_if_necessary(entry->get_author());  // testing ***
            case 1:
                  return entry->get_title();
            case 2:
                  return entry->get_year();
            case 3:
                 return entry->get_key();
            case 4:
                 return entry->get_keywords();
            case 5:
                 return entry->get_size();
            case  6:
                 return entry->get_on_board_entries().count();
            case 7:
                 return entry->get_folder();
            case 8:
                 return entry->get_filenamestem();
            case 9:
                 return entry->get_info("date");
            case 10:
                 return entry->get_info("lastread");
            case 11:
                 return entry->get_info("date_entry_created");
         }
     }
     if (role == Qt::FontRole && entry->get_on_board_entries().size() > 0){
         QFont font;
         font.setBold(true);
         return font;
     }

     if (role==Qt::ForegroundRole && entry->get_on_board_entries().size() > 0 ){
         QBrush brush;
         brush.setColor(Qt::GlobalColor(Qt::darkBlue));
         return brush;
     }
     if (role==Qt::CheckStateRole){
         if (index.column()==5){
             if (entry->selected_for_deletion()){
                 return Qt::Checked;
             }
             return Qt::Unchecked;
         }
     }
     return QVariant();
 }
QVariant BiblioTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Author");
            case 1:
                return tr("Title");
            case 2:
                return tr("Year");
            case 3:
                return tr("Item key");
            case 4:
                return tr("Key words");
            case 5:
                return tr("Size");
            case 6:
                return tr("File count");
            case 7:
                return tr("Folder");
            case 8:
                return tr("Filename");
            case 9:
                return tr("Date");
            case 10:
                return tr("Last read");
            case 11:
                return tr("Date entry created");
            default:
                    return QVariant();
        }
    }
    return QVariant();
}
bool BiblioTableModel::insertRows(int position, int how_many_rows, const QModelIndex &index)
{

    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + how_many_rows - 1);

    for (int row=0; row < how_many_rows; row++) {
        Entry * entry = new Entry();
        m_entries.insert(position, entry);
    }
    endInsertRows();
    return true;

}
bool BiblioTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    /*
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        m_entries.removeAt(position);
    }

    endRemoveRows();
    */
    return true;
}
bool BiblioTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        if (index.isValid() && role == Qt::EditRole) {
                int row = index.row();
                Entry * p = m_entries.value(row);
                if (index.column() == 0)
                    {p->set_author( value.toString());}
                    else {
                        if (index.column() == 2) {
                            p->set_title( value.toString() );}
                        else {
                            return false;}
                    }
                m_entries.replace(row, p);
                emit(dataChanged(index, index));
                return true;
        }
        return false;
}
