#ifndef ENTRY_H
#define ENTRY_H
#include <QMap>
#include <QString>

class Entry {
    //friend class EntryModel;

    QMap<QString,QString>   info;
    //QString               key;
    int                     size;
    bool                    m_selected_for_deletion;
    //QDateTime m_creation_time;`
    QString                 m_filename_full;
    QStringList             m_multiple_filenamefulls;
    QStandardItem *         m_top_view_size_item;
    QStandardItem *         m_top_view_filename_item;
    QTableWidgetItem *      m_bottom_view_size_item;
    QTableWidgetItem *      m_bottom_view_filename_item;
    QList<Entry*>           m_links_to_on_board_entries;
    QList<Entry*>           m_links_to_bib_entries;
    QColor                  m_temporary_color;
public:
    Entry();
    Entry(const Entry&);
    Entry (QFileInfo &);
    Entry(QString this_key) {info.insert("key", this_key); }
    Entry(QString filename, QString path, qint64 size);
    Entry(QString filename, QString path, int size) ;
    ~Entry();

    QString display();

    void append_keywords(QString k){info["keywords"] = get_keywords() + " " + k;}
    void add_to_bib_entries(Entry* entry){m_links_to_bib_entries.append(entry);}
    void add_to_onboard_entries(Entry* entry);
    void add_keywords(QTableWidget *);
    bool contains_info(QString key){return info.contains(key);}
    QString get_info(QString);
    int     get_size()         {return size;}
    QString get_key()          {if (info.contains("key")) {return info["key"];} else{return QString();}}
    QString get_title()        {if (info.contains("title")) {return info["title"];} else{return QString();}}
    QString get_author()       {if (info.contains("author")) {return info["author"];} else{return QString();}}
    QString get_year()         {if (info.contains("year")) {return info["year"];} else{return QString();}}
    QString get_folder()       {if (info.contains("folder")) {return info["folder"];} else{return QString();}}
    QString get_filenamefull();
    QString get_keywords()     {if (info.contains("keywords")) {return info["keywords"];} else{ return QString();}}
    QStringList* get_multiple_filenamefulls() {return &m_multiple_filenamefulls;}
    QString get_filenamestem() {if (info.contains("filenamestem")) {return info["filenamestem"];} else{return QString();}}
    QColor get_temporary_color() {return m_temporary_color;}
    bool    if_linked_to_onboard_entry();
    bool    if_linked_to_biblio_entry();
    void write_bibentry_to_bibtex( QTextStream & ,QStringList &);
    QList<Entry*> get_on_board_entries(){return m_links_to_on_board_entries;}
    QList<Entry*> get_links_to_bib_entries() {return m_links_to_bib_entries;}
    //void mark_bottom_view_entry_as_matched_to_biblio();
    void remove_bottom_view_links();
    void read_json(QJsonObject &);
    void set_key(QString s) {info["key"] = s;}
    void    set_keywords(QString s) {info["keywords"] = s;}
    void set_title(QString s)  {info["title"] = s;}
    void set_author(QString s) {info["author"] = s;}
    void set_year(QString s) {info["year"] = s;}
    void set_filename_full(QString s);// {info["filenamefull"] = s;}
    void set_filename_stem(QString s) {info["filenamestem"] = s;}
    void set_folder(QString folder) {info["folder"] = folder;}
    void set_size(int s) {size = s;}
    bool selected_for_deletion() {return m_selected_for_deletion;}
    void set_selected_for_deletion(bool value) {m_selected_for_deletion = value;}
    void set_top_view_size_item (QStandardItem* item ){m_top_view_size_item = item;}
    void set_bottom_view_size_item(QTableWidgetItem* item ) {m_bottom_view_size_item = item;}
    void set_top_view_filename_item (QStandardItem* item ){m_top_view_filename_item = item;}
    void set_bottom_view_filename_item(QTableWidgetItem* item ) {m_bottom_view_filename_item = item;}
    void set_info(QString key,QString value); //{info[key] = value;}
    void set_info(QString key, int this_size) {size = this_size; }
    void set_size_item(int);
    void set_temporary_color(QColor color) {m_temporary_color = color;}
    void set_filename_item_bottom(QString);
    // keys: key, title, author, year, filenameFull, filenameStem



};
#endif // ENTRY_H
