#ifndef FILES_MATCH_MODEL_H
#define FILES_MATCH_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include "file.h"

class files_match_model: public QAbstractTableModel
{
    Q_OBJECT

    QMultiMap<int, File*  >  * m_size_matches;

public:
    files_match_model(QMultiMap<int, File*> *);

    int             get_match_count() { return m_size_matches->count(); }
    QList<File*>    get_match(int n)  { return m_size_matches->values(n); }

    int             rowCount(const QModelIndex &index ) const {return m_size_matches->count();}
    int             columnCount(const QModelIndex &index) const { return 5;}
    QVariant        data(const QModelIndex & index, int role )const;
};

#endif // FILES_MATCH_MODEL_H
