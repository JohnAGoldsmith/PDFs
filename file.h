#ifndef FILE_H
#define FILE_H
#include <QMap>
#include <QDate>
#include <QString>
#include <QStandardItem>
#include <QFileInfo>
#include <QDir>
#include "Entry.h"


class File
{   QMap<QString,QString>   info;
    int                     m_size;
    bool                    m_selected_for_deletion;
    QString                 m_filename;
    QString                 m_folder;
    QList<Entry*>           m_links_to_bib_entries;
    QColor                  m_temporary_color;
    QString                 m_date_created;
    QString                 m_date_last_modified;
    QString                 m_date_lastRead;

public:
    File();
     File(const File&);
    File (QFileInfo &);
    File(QString this_key) {info.insert("key", this_key); }
    File(QString filename, QString path, qint64 size);
    File(QString filename, QString path, int size) ;
    ~File();

    QString display();

    void    append_keywords(QString k){info["keywords"] = get_keywords() + " " + k;}
    void    add_to_bib_entries(Entry* entry){m_links_to_bib_entries.append(entry);}
    void    add_to_onboard_entries(Entry* entry);
    bool    contains_info(QString key){return info.contains(key);}

    QString   get_date_created() {return m_date_created;}
    QString   get_date_last_read() {return m_date_lastRead;}
    QString get_date_last_modified() {return m_date_last_modified;}
    QString get_info(QString);
    int     get_size()         {return m_size;}
    QString get_folder()       {return m_folder;}
    QString get_filename()     {return m_filename;}
    QString get_keywords()     {if (info.contains("keywords")) {return info["keywords"];} else{ return QString();}}
    //QString get_filename() {if (info.contains("filename")) {return info["filename"];} else{return QString();}}
    //QString get_filename() { return m_filename;}
    QColor  get_temporary_color() {return m_temporary_color;}
    QList<Entry*> get_links_to_bib_entries() {return m_links_to_bib_entries;}

    bool    if_linked_to_biblio_entry();
    void    remove_bottom_view_links();
    void    read_json(QJsonObject &);

    void    set_key(QString s) {info["key"] = s;}
    void    set_keywords(QString s) {info["keywords"] = s;}
    void    set_title(QString s)  {info["title"] = s;}
    void    set_author(QString s) {info["author"] = s;}
    void    set_year(QString s) {info["year"] = s;}
    void    set_filename_full(QString s);// {info["filenamefull"] = s;}
    void    set_filename_stem(QString s) {info["filename"] = s;}
    void    set_folder(QString folder) {info["folder"] = folder;}
    void    set_size(int s) {m_size = s;}
    bool    selected_for_deletion() {return m_selected_for_deletion;}
    void    set_selected_for_deletion(bool value) {m_selected_for_deletion = value;}
    void    set_info(QString key,QString value); //{info[key] = value;}
    void    set_info(QString key, int this_size) {m_size = this_size; }
    void    set_size_item(int);
    void    set_temporary_color(QColor color) {m_temporary_color = color;}
    void    set_filename_item_bottom(QString);

    void    write_bibentry_to_bibtex( QTextStream & ,QStringList &);



};

#endif // FILE_H
