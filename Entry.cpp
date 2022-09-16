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

class List;


Entry::Entry(){
    size = 0;
    m_top_view_size_item = NULL;
    m_top_view_filename_item = NULL;
    m_bottom_view_size_item = NULL;
    m_bottom_view_filename_item = NULL;
    //m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
     m_selected_for_deletion = false;
     m_temporary_color = QColorConstants::Black;
}
Entry::Entry(const Entry& entry){
    size = entry.size;
    foreach (QString label, entry.info.keys()){
        info[label] = entry.info[label];
    }
}
Entry::~Entry(){

}
Entry::Entry(QString stem, QString folder, qint64 a_size)
{
    int this_size = static_cast<int>(a_size);
    info["folder"] = folder;
    info["filenamestem"] = stem;
    info["filenamefull"] = folder + "/" + stem;
    size = this_size;
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
  size = this_size;
  QString filenameFull = folder + "/" + stem;
  set_folder(folder);
  set_filenameStem(stem);
  set_filenameFull(filenameFull);
  /*
  m_top_view_size_item = NULL;
  m_top_view_filename_item = NULL;
  m_bottom_view_size_item = NULL;
  m_bottom_view_filename_item = NULL;
  */
  //m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
   m_selected_for_deletion = false;
}

void Entry::set_info(QString this_key,QString this_value)
{   if (this_key == "size"){
        size = this_value.toInt();
    } else{
        info.insert(this_key, this_value);
    }
}
void Entry::set_filenameFull(QString s) {
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



void Entry::add_to_onboard_entries(Entry *bottom_entry){
    m_links_to_on_board_entries.append(bottom_entry);
}
// Don't use this next function??
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
void Entry::mark_bottom_view_entry_as_matched_to_biblio(){
     QTableWidgetItem* item_bottom = m_bottom_view_size_item;
     item_bottom->setForeground(Qt::GlobalColor(Qt::darkBlue));

}
void Entry::color_bottom_view_item_for_size(){
    m_bottom_view_size_item->setForeground(QBrush(QColor(Qt::blue)));
    m_bottom_view_filename_item->setForeground(QBrush(QColor(Qt::blue)));
    QFont font;
    font.setBold(true);
    m_bottom_view_filename_item->setFont(font);
}
QString Entry::get_filenamefull() {
    QString filenamefull;
    if (info.contains("filenamefull")){
        filenamefull = info["filenamefull"];
    }
    {if (filenamefull.length() > 0){
        return filenamefull;
        }
    }
    if (info["filenamestem"].length()>0 &&
         info["folder"].length() > 0){
            return info["folder"]  + "/" + info["filenamestem"];
    }
    return QString();
}
void Entry::color_bottom_view_item_for_filename(){
    QTableWidgetItem* item_bottom = m_bottom_view_filename_item;
    item_bottom->setForeground(QColorConstants::Blue);
}
// deprecated
void Entry::color_top_view_item_for_size(){
}
//deprecated
void Entry::color_top_view_item_for_filename(){
}


void Entry::set_filename_item_bottom(QString filename){
    m_bottom_view_filename_item->setText(filename);
}
