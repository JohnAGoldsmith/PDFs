#include <QModelIndex>
#include "files_match_model.h"

files_match_model::files_match_model(QMultiMap<int, File*> * map) {
    m_size_matches = map;
}

QVariant files_match_model::data(const QModelIndex & index, int role ) const {

}
