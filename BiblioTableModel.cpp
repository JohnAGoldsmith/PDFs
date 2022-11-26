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
#include "BiblioTableModel.h"
#include "Entry.h"


//class List;

/*
BiblioTableModel::BiblioTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}
*/
BiblioTableModel::BiblioTableModel(QList< Entry* > entrylist, QObject *parent)
    : QAbstractTableModel(parent){

}
Entry*  BiblioTableModel::get_entry_by_size(int size) {
    if ( m_data_by_size.contains(size)){
         return m_data_by_size.value(size);
    } else{
        return nullptr;
    }
}

void BiblioTableModel::register_all_entries()
{
     foreach (Entry* entry, m_entries){
        register_entry(entry);
    }
}
void BiblioTableModel::register_entry(Entry* entry){
    register_biblioentry_by_key(entry);
    register_biblioentry_by_size(entry);
    register_biblioentry_by_filenamestem(entry);
    register_biblioentry_by_fullfilename(entry);
};
void BiblioTableModel::register_biblioentry_by_key(Entry* entry){
    QString key = entry->get_key();
    if (m_data_by_key_multiple.contains(key)){
        m_data_by_key_multiple.insert(key, entry);
    } else {
        if (m_data_by_key.contains(key) ) {
              m_data_by_key_multiple.insert(key, m_data_by_filenamestem[entry->get_filenamestem()]);
              m_data_by_key_multiple.insert(key, entry);
            }
           else {
            m_data_by_key.insert(key, entry);
           }
    }
}
QList<Entry*> BiblioTableModel::get_multiple_entries_from_size(int size) {
    if (m_data_by_size_multiple.contains(size)) {
            return m_data_by_size_multiple.values(size);
    }  else
    {
       return QList<Entry*>();
    }
}
QList<Entry*>  BiblioTableModel::get_multiple_entries_from_filename_stem(QString stem){
    if (m_data_by_filenamestem_multiple.contains(stem)){
        return m_data_by_filenamestem_multiple.values(stem);
    } else{
        return QList<Entry*>();
    }
}


void BiblioTableModel::register_biblioentry_by_size(Entry * entry ){
    int size = entry->get_size();
    if (size == 0) {return;}
    if (m_data_by_size_multiple.contains(size)){
        m_data_by_size_multiple.insert(size, entry);
    } else {
        if (m_data_by_size.contains(size) ) {
              m_data_by_size_multiple.insert(size, m_data_by_size[size]);
              m_data_by_size_multiple.insert(size, entry);
            }
           else {
              m_data_by_size.insert(size, entry);
           }
    }
}
void BiblioTableModel::register_biblioentry_by_fullfilename(Entry* entry){
    QString full_filename = entry->get_filenamefull();
    if (m_data_by_fullfilename_multiple.contains(full_filename) ) {
        m_data_by_fullfilename_multiple.insert(full_filename, entry);
    } else {
        if (m_data_by_fullfilename_multiple.contains( full_filename ) ) {
              m_data_by_fullfilename_multiple.insert( full_filename, m_data_by_fullfilename[entry->get_filenamefull()]);
              m_data_by_fullfilename_multiple.insert( full_filename, entry);
            }
         else {
              m_data_by_fullfilename.insert(full_filename, entry);
         }
    }
}
void BiblioTableModel::register_biblioentry_by_filenamestem(Entry* entry){
    QString stem_of_filename = entry->get_filenamestem();
    if (m_data_by_filenamestem_multiple.contains(stem_of_filename)){
        m_data_by_filenamestem_multiple.insert(stem_of_filename, entry);
    } else {
        if (m_data_by_filenamestem.contains(stem_of_filename ) ) {
              m_data_by_filenamestem_multiple.insert(stem_of_filename, m_data_by_filenamestem[stem_of_filename]);
              m_data_by_filenamestem_multiple.insert(stem_of_filename, entry);
            }
         else {
              m_data_by_filenamestem.insert(stem_of_filename, entry);
         }
    }
}
/*
QList<Entry*> BiblioTableModel::contains_filenamestem(const QString & stem ){
    if (m_data_by_filenamestem.contains(stem)){
        return m_data_by_filenamestem.values(stem);
    } else{
        return QList<Entry*>();
    }
}*/
bool BiblioTableModel::contains_filenamestem(const QString & stem){
    return m_data_by_filenamestem.contains(stem);
}

void BiblioTableModel::add_entry(Entry *entry){
    m_entries.append(entry);
    // also add to all the QMaps;


}
int BiblioTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {return 0;}
    return m_entries.size();
}
int BiblioTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {return 0;}
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
     if (!index.isValid()) { return QVariant(); }
     //if (role != Qt::DisplayRole) {return QVariant();}
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
     if (role == Qt::BackgroundRole && entry->if_linked_to_onboard_entry() ){
             QColor col1(0,255,255);
             QVariant var2 = col1;
             return var2;
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
    QStringList labels;
    labels << "Author" << "Title" << "Year" << "Item key" << "Key words" << "Size" << "File count" << "Folder" <<
              "Filename" << "Date" << "Last read" << "Date entry created";
    if (orientation == Qt::Horizontal) {
        if (section < 0 || section > 11) { return QVariant(); }
        else {
            return labels[section];
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
{    if (index.isValid() && role == Qt::EditRole) {
                int row =  m_proxyModel->mapToSource(index).row();
                Entry * entry = m_entries.value(row);
                if (index.column() == 1){
                    switch (index.row()) {
                        case 3:{
                            entry->set_author(value.toString());
                            break;
                        }
                        case 4:{
                            entry->set_title(value.toString());
                            break;
                        }
                        case 5:{
                            entry->set_info("date",value.toInt());
                            break;
                        }
                        default:{

                        }
                    }
                }
                emit dataChanged(index, index);
                qDebug() << 262 << "inside setData"<< entry->display();
                return true;
     }
     return false;
}
void BiblioTableModel::update_data_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry){
    m_data_by_fullfilename.remove(full_old_name);
    m_data_by_fullfilename[full_new_name] = entry;
}
