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

EntryModel::EntryModel(Entry* entry, Widget * main_widget){
    m_parent = main_widget;
    m_entry = entry;


}
EntryModel::~EntryModel(){

}
void EntryModel::display(Entry* entry){
    m_entry = entry;
}
int EntryModel::rowCount(const QModelIndex &index ) const
{
   Q_UNUSED(index);
   return  m_parent->m_bibliography_labels.count();
}

int EntryModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 2;
}
bool EntryModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 1){
            int row = index.row();
            QString label = get_bibliography_labels().at(row);
            if (label == "size"){
                m_entry->set_size(value.toInt());
                qDebug() << "size" << value.toInt();
            } else{
                qDebug() << "label" << label;
                qDebug() << "value.string" << value.String;
                //m_entry->set_info(label, value.String);
                m_entry->set_info(label, value.toString());
                emit dataChanged(index,index);
                return true;
            }
        }
        else{
            return false;
        }
        //stringList.replace(index.row(), value.toString());
        //emit dataChanged(index, index, {role});
        //qDebug() << "entry model" << value;
        return true;
    }
    return false;
}


QVariant EntryModel::data(const QModelIndex & index, int role )const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= get_bibliography_labels().size() || index.row() < 0) {
        return QVariant();
    }
    int row = index.row();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
         if ( index.column() == 1) {
            if (! m_entry ) { return QVariant(); }
            if (get_bibliography_labels().at(row) == "size"){
                return QVariant(m_entry->get_size());
            }
            return  QVariant( m_entry->get_info(  get_bibliography_labels().at(row) ) );
         }
         if (index.column() == 0){
            return QVariant (get_bibliography_labels().at(row));
         }
    } // end of role 0, which is displayed material

    return QVariant();
}

Qt::ItemFlags EntryModel::flags(const QModelIndex &index) const {
         return Qt::ItemIsEditable | Qt::ItemIsSelectable |
                 Qt::ItemIsEnabled  |   QAbstractTableModel::flags(index);
}

