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
#include "BiblioTableModel.h"
#include "FilesModel.h"
#include "entry_match_model.h"
#include "Entry.h"

EntryMatchModel::EntryMatchModel(BiblioTableModel* biblio_model, FilesModel* file_model, QWidget* parent){
   m_biblio_model = biblio_model;
   m_file_model = file_model;
};


void EntryMatchModel::match_filestems(){
    // for each entry in biblioEntries, find all the files in onboardFiles that match the filestem-name.
    QList<File*> file_list;
    if (!m_biblio_model) {return;}
    if (!m_file_model) {return;}
    foreach (Entry* entry, m_biblio_model->get_entries()){
        if (entry->get_size() > 0) {continue;}
        if (entry->get_filename().length() == 0){ continue;}
        QString filename = entry->get_filename();
        if (m_file_model->contains_multiple_entries_with_filename( filename ) ) {
            file_list = m_file_model->get_files_with_filename( filename );
            foreach (File* file,  file_list){
                QPair<Entry*, File*> * pair = new QPair< Entry*, File*>(entry, file);
                m_entries.append(pair);
            }
        } // end of multiple matches
        else { File* file = m_file_model->get_file_with_filename(filename);
               if (file){
                  QPair<Entry*,File*>* pair = new QPair <Entry*,File*>(entry, file);
                  m_entries.append(pair);
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
    // todo to do
    Entry* entry = m_entries.at(row)->first;
    if (!entry) return QVariant();
    if (role == 0) {
        switch (index.column()){
        case 0:
            return QVariant (index.row()+1);
         case 1:
            return QVariant(entry->get_filename() );
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



#endif
