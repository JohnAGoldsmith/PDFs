#include "file.h"

File::File() {
    int i  = 1;
}



File::File (QFileInfo& info){
    m_filename  = info.fileName();
    m_folder = info.path();
    m_size = info.size();
    m_date_created = info.birthTime().toString("yyyy MMMM d");
    m_date_last_modified =  info.lastModified().date().toString("yyyy MMMM d");
    m_date_lastRead = info.lastRead().toString("yyyy MMMM d");
}

bool File::if_linked_to_biblio_entry(){
    if (m_links_to_bib_entries.size() > 0) {return true;}
    return false;
}
