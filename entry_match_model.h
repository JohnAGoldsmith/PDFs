#ifndef ENTRY_MATCH_MODEL_H
#define ENTRY_MATCH_MODEL_H

#include <QAbstractTableModel>
#include "BiblioTableModel.h"
#include "FilesModel.h"



class EntryMatchModel: public QAbstractTableModel{
    Q_OBJECT

    BiblioTableModel *                  m_biblio_model;
    FilesModel *                        m_file_model;
    QList<QPair< Entry*, File* > * >    m_entries;
public:
    EntryMatchModel(BiblioTableModel * biblio_model, FilesModel *, QWidget*);

    void     match_filestems();
    int      rowCount(const QModelIndex &index ) const;
    int      columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex & index, int role )const;
};


#endif // ENTRY_MATCH_MODEL_H
