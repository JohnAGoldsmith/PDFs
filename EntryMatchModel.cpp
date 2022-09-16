#ifndef ENTRYMATCHMODEL_H
#define ENTRYMATCHMODEL_H

#include <QWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStringList>
#include <QSettings>
#include <QKeyEvent>
#include <QApplication>
#include <QJsonObject>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QList>
#include <QComboBox>
#include <QMultiMap>
#include <QShortcut>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QFileSystemModel>
#include "widget.h"


EntryMatchModel::EntryMatchModel(BiblioTableModel* biblio_model, EntriesModel* file_model, QWidget* parent){
   m_biblio_model = biblio_model;
   m_file_model = file_model;
};


void EntryMatchModel::match_filestems(){
    // for each entry in biblioEntries, find all the entries in onboardFiles that match the filestem-name.
    QList<Entry*> entry_list;
    foreach (Entry* entry, m_biblio_model->get_entries()){
        if (entry->get_size() > 0) {continue;}
        if (entry->get_filenamestem().length() == 0){ continue;}
        QString filename = entry->get_filenamestem();
        if (m_file_model->contains_multiple_entries_with_filenamestem( filename ) ) {
            entry_list = m_file_model->get_entries_with_filenamestem( filename );
            m_entries.append(entry);
            foreach (Entry* entry2,  entry_list){
                m_entries.append(entry2);
            }
        } // end of multiple matches
        else { Entry* entry3 = m_file_model->get_entry_with_filenamestem(filename);
               if (entry3){
                       m_entries.append(entry);
                       m_entries.append(entry3);
               }
        }
    }
}

int EntryMatchModel::rowCount(const QModelIndex &index ) const
{
   Q_UNUSED(index);
   return  m_entries.count();
}

int EntryMatchModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 6;
}

QVariant EntryMatchModel::data(const QModelIndex & index, int role )const
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

EntryMatchView::EntryMatchView () {

}



#endif
