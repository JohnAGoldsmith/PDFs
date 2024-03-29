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
#include <EGL/egl.h>
#include "Entry.h"
#include "file.h"
//class List;


Entry::Entry(){
    m_size = 0;
    //m_top_view_size_item = NULL;
    //m_top_view_filename_item = NULL;
    //m_bottom_view_size_item = NULL;
    //m_bottom_view_filename_item = NULL;
    //m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
     m_selected_for_deletion = false;
     m_temporary_color = QColorConstants::Black;

}
Entry::Entry(const Entry& entry){
    m_size = entry.m_size;
    foreach (QString label, entry.info.keys()){
        info[label] = entry.info[label];
    }
}
Entry::Entry(File* file){
    m_size = file->get_size();
    m_filename = file->get_filename();
    m_folder = file->get_folder();
    info["folder"] = file->get_folder();
    info["date_created"] = file->get_date_created();
    info["date_last_read"] = file->get_date_last_read();
    info["date_last_modified"] = file->get_date_last_modified();
}
/*
Entry::Entry (QFileInfo& info){
    QString filename = info.fileName().trimmed();
    QString folder = info.dir().absolutePath();
    QString filenamefull = folder + "/" + filename;
    int     size = info.size();
    set_folder(folder);
    set_filename_stem(filename);
    set_info("filenamefull", filenamefull);
    set_info("date", info.lastModified().date().toString("yyyy MM dd"));
    set_info("lastread", info.lastRead().date().toString("yyyy MM dd"));
    set_size(size);
}*/
Entry::~Entry(){

}
Entry::Entry(QString stem, QString folder, qint64 a_size)
{
    int this_size = static_cast<int>(a_size);
    info["folder"] = folder;
    info["filename"] = stem;
    info["filenamefull"] = folder + "/" + stem;
    m_size = this_size;
    /*
    m_top_view_size_item = NULL;
    m_top_view_filename_item = NULL;
    m_bottom_view_size_item = NULL;
    m_bottom_view_filename_item = NULL;
    */
    //m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
     m_selected_for_deletion = false;
     m_temporary_color = nullptr;
}
Entry::Entry(QString stem, QString folder, int this_size)
{
  m_size = this_size;
  QString filenameFull = folder + "/" + stem;
  set_folder(folder);
  set_filename_stem(stem);
  set_filename_full(filenameFull);
  set_key (stem);
  /*
  m_top_view_size_item = NULL;
  m_top_view_filename_item = NULL;
  m_bottom_view_size_item = NULL;
  m_bottom_view_filename_item = NULL;
  */
  //m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
   m_selected_for_deletion = false;
}
/*
bool Entry::if_linked_to_biblio_entry(){
    if (m_links_to_bib_entries.size() > 0) {return true;}
    return false;
}*/
bool Entry::if_linked_to_onboard_entry(){
    if (m_links_to_files.size() > 0){return true;}
    return false;
}
void Entry::set_info(QString this_key,QString this_value)
{   if (this_key == "size"){
        m_size = this_value.toInt();
    } else{
        info.insert(this_key, this_value);
    }
}
void Entry::set_filename_full(QString s) {
    if (get_filenamefull().length() == 0){
        info["filenamefull"] = s;
    } else{
        m_multiple_filenamefulls.append(s);
    }
}
QString Entry::display(){
    QString result;
    result = "[size: " + QString::number(get_size()) + "]  ";
    foreach (QString key, info.keys()){
        result += "[" +  key + " : " + info[key] + "]  " ;
    }
    return result;
}
QString Entry::get_info(QString key){
    if (info.contains(key)) {
        return info[key];
    }
    else {
        return QString();
    }
}

void Entry::ingest_file(File* file){
    set_size(file->get_size());
    set_filename_stem(file->get_filename());
    set_folder(file->get_folder());
}

/*
void Entry::add_to_onboard_entries(Entry *bottom_entry){
    m_links_to_on_board_entries.append(bottom_entry);
}
*/
// Don't use this next function??
/*
void Entry::set_size_item(int size){
    QStandardItem * item;
    if (m_top_view_size_item) {
        m_top_view_size_item->setText(QString::number(size));
    }
    if (m_bottom_view_size_item) {
       m_bottom_view_size_item->setText(QString::number(size));
    }
}
void Entry::add_keywords(QTableWidget* middlewidget){
    int row_for_keywords = 7;
    set_keywords(middlewidget->item(row_for_keywords,1)->text());
}
*/

QString Entry::get_filenamefull() {
    QString filenamefull;
    if (info.contains("filenamefull")){
        filenamefull = info["filenamefull"];
    }
    if (filenamefull.length() > 0){
        return filenamefull;
    }
    if (info["filename"].length()>0 &&
         info["folder"].length() > 0){
            return info["folder"]  + "/" + info["filename"];
    }
    return QString();
}


/*
void Entry::set_filename_item_bottom(QString filename){
    m_bottom_view_filename_item->setText(filename);
}
*/
