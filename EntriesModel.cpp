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
#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <EGL/egl.h>
class List;





EntriesModel::EntriesModel(Widget * main_widget){
    m_parent = main_widget;
    //m_entry = main_widget->m_entry_in_bottom_table;
    m_proxyModel = new MySortFilterProxyModel();
    m_proxyModel->setSourceModel(this);
}
EntriesModel::~EntriesModel(){

}
bool EntriesModel::contains_size(int this_size){
    if (m_data_by_size.contains(this_size)) {
        return true;
    }
    return false;
}
bool EntriesModel::contains_filename_stem(QString stem){
    if (m_data_by_filename_stem.contains(stem)){
        return true;
    }
    return false;
}
QVariant EntriesModel::headerData(int section, Qt::Orientation orientation, int role) const{
  QStringList horizontal_headers;
  horizontal_headers << " " << "File name" << "Folder" << "Size" << "Date created" << "Date accessed";
  if (role == Qt::DisplayRole)
  {
    if (orientation == Qt::Horizontal) {
        if (section >= 0 && section < horizontal_headers.size()){
            return horizontal_headers[section];
        } else {
          return QString("test");
        }
    }
  }
  return QVariant();

};
void EntriesModel::addEntry(Entry * entry){
    beginInsertRows(QModelIndex(), m_entries.size(), m_entries.size());
    m_entries.append(entry);
    endInsertRows(); //notify views that you're done with modifying the underlying data
    register_entry(entry);
}
void EntriesModel::register_entry(Entry* entry){
    register_entry_by_size(entry);
    register_entry_by_filename_full(entry);
}
void EntriesModel::register_entry_by_filename_full(Entry* entry){
    m_data_by_filenamefull[entry->get_filenamefull()] = entry;
}
void EntriesModel::register_entry_by_size(Entry * entry){
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
int EntriesModel::rowCount(const QModelIndex &index ) const
{
//   qDebug() << 66 << m_entries.count();
   Q_UNUSED(index);
   return  m_entries.count();
}

int EntriesModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 6;
}
bool EntriesModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row =  m_proxyModel->mapToSource(index).row();
        Entry * entry = m_entries.value(row);
        if (index.column() == 1){
            switch (index.row()) {
                case 0:{
                    entry->set_filename_stem(value.toString());
                    break;
                }
                case 1:{
                    entry->set_folder(value.toString());
                    break;
                }
                case 3:{
                    entry->set_author(value.toString());
                    break;
                }
                case 4:{
                    entry->set_title(value.toString());
                    break;
                }
                case 5:{
                    entry->set_year(value.toString());
                    break;
            }
            }

        }
    }
    return false;
}


QVariant EntriesModel::data(const QModelIndex & index, int role )const
{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    if (m_entries.count() == 0){
        return QVariant();
    }
    Entry* entry = m_entries.at(row);
    if (!entry) return QVariant();
    if (role == 0) {
        switch (index.column()){
        case 0:
            return QVariant (index.row()+1);
         case 1:
            return QVariant(entry->get_filenamestem() );
            break;
        case 2:
           return QVariant(entry->get_folder());
           break;
        case 3:
           return QVariant(entry->get_size());
           break;
        case 4:
           return QVariant(entry->get_info("date" ));
           break;

        default:
            return  QVariant(entry->get_info("lastread"));
        }
    }
    if (role == Qt::BackgroundRole && entry->if_linked_to_biblio_entry()){
            QColor col1(0,255,255);
            QVariant var2 = col1;
            return var2;
    }

     // end of role 0, which is displayed material

    return QVariant();
}

EntriesView_onboard_files::EntriesView_onboard_files(){
    setSortingEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    //m_bottomTableView->horizontalHeader()->setMaximumSectionSize(int);
}
