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
 }
int EntriesModel::rowCount(const QModelIndex &index ) const
{
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
           return QVariant(entry->get_info("date_created" ));
           break;

        default:
            return  QVariant( "default");
        }
    }

     // end of role 0, which is displayed material

    return QVariant();
}
