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





int function2 (QString line, Entry * entry, int n){ //file_info * this_file_info){
    if (line.endsWith("]")){line.chop(1);}
    if (line.startsWith("[ ")) { line.remove(0,2);}
    QStringList pieces = line.split(".pdf,");
    QString filenamestem = pieces[0];
    QString fullfilename = pieces[1];
    QString filesize = pieces[2];
    int i_filesize = filesize.toUInt();
    if (fullfilename.startsWith(" ../../")){
        fullfilename = fullfilename.mid(7);
    }
    filenamestem += ".pdf";
    fullfilename += ".pdf";
    QString key = "possible_file::" + QString::number(n);
    entry->set_info(key, fullfilename );
    //qDebug() << 276 << key  <<  fullfilename;
    return i_filesize;
}
int function3 (QString line, Entry * entry){ //file_info * this_file_info){
     Q_UNUSED(entry);
    if (line.endsWith("]")){line.chop(1);}
    if (line.startsWith("[ ")) { line.remove(0,2);}
    QStringList pieces = line.split(".pdf,");
    QString filenamestem = pieces[0];
    QString fullfilename = pieces[1];
    QString filesize = pieces[2];
    int i_filesize = filesize.toUInt();
    return i_filesize;
}
void Entry::read_json(QJsonObject & json){
    foreach (QJsonValue key_json, json.keys()){
        QString key = key_json.toString();
        qDebug() << 349 << key;
        if (key == "size"){
            size = json[key].toInt();
        }else{
            info[key] = json[key].toString() ;
        }
        qDebug() << 354 << key << info[key] << get_size();
    }
}
void Widget::read_JSON_file_new(QString filename){
    QJsonObject json_bibliography;
    QString foldername;
    if (filename.isEmpty()){
        filename= QFileDialog::getOpenFileName(this, "Choose File", m_json_folder, "JSON files (*.json)");
        if(filename.isEmpty())
            return;
    }
    QFile fileIn(filename);
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    int slash_loc = filename.lastIndexOf("/") + 1;
    foldername = filename.left(slash_loc);
    m_json_folder= foldername;
    QByteArray bytearray = fileIn.readAll();
    QJsonParseError parseError;
    QJsonDocument json_doc =  QJsonDocument::fromJson(bytearray, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    } else {
            json_doc.toJson(QJsonDocument::Compact);
    }

    m_settings.setValue("jsonfilename", filename);
    m_settings.setValue("jsonfoldername", foldername);
    fileIn.close();

    bool NewFormatFlag = true;
    if (NewFormatFlag){
        if(!json_doc.isArray()){
                qDebug() << "JSON doc root is not an array.";
        }
        QJsonObject  json_settings;
        json_settings = json_doc[0].toObject();
        if (json_settings.contains("m_directory_view_root") && json_settings["m_directory_view_root"].isString() ){
            m_directory_view_root = json_settings["m_directory_view_root"].toString();
            m_file_system_model->setRootPath(m_directory_view_root);
            //m_file_system_model->dataChanged();
        }
        json_bibliography = json_doc[2].toObject();
    }
    if(json_bibliography.isEmpty()){
        qDebug() << "The array is empty";
    }
    foreach (QString item_key, json_bibliography.keys()){
        if(! json_bibliography[item_key].isObject()) {continue;}
        QJsonObject j_this_entry =  json_bibliography[item_key].toObject();
        if (j_this_entry.count() > 0){
                Entry* entry = new Entry();
                foreach (QString entry_internal_key, j_this_entry.keys()){
                    if (entry_internal_key == "filenamestem" ||
                        entry_internal_key == "folder"){
                        continue;
                    }
                    if (entry_internal_key == "size"){
                        entry->set_size(j_this_entry["size"].toInt());
                    } else {
                        QString value = j_this_entry[entry_internal_key].toString();
                        entry->set_info(entry_internal_key, value);
                    }
                }
                promote_file_from_preferred_location(entry);
                biblioModel->add_entry(entry);
        }
    }
    biblioModel->register_all_entries();
    biblioModel->m_proxyModel = new MySortFilterProxyModel () ;
    biblioModel->m_proxyModel->setSourceModel( biblioModel );

    m_topTableView->setModel( biblioModel->m_proxyModel );
    m_topTableView->setColumnWidth(0,300);
    m_topTableView->setColumnWidth(1,400);
    m_topTableView->setColumnWidth(2,100);
    m_topTableView->setColumnWidth(3,400);
    m_topTableView->setColumnWidth(4,300);
    m_topTableView->setColumnWidth(5,150);
    m_topTableView->setColumnWidth(6,100);
    m_topTableView->setColumnWidth(7,400);
    m_topTableView->setColumnWidth(8,400);

    // What follows has to be done *after* the entries have been loaded: this is the List information.
   if (false && NewFormatFlag){
       QJsonArray json_lists_array;
       json_lists_array = json_doc[1].toArray();

       QJsonObject json_list_object ;
       List* list;
       for (int n = 0; n < json_lists_array.size(); n++){
           json_list_object = json_lists_array[n].toObject();
           QString list_name = json_list_object["list_name"].toString();
           if (m_Lists_map.contains(list_name)){
               list = m_Lists_map[list_name];
           } else{
               list = new List (json_list_object["list_name"].toString());
           }
           QJsonArray json_entries;
           json_entries = json_list_object["entries"].toArray();
           //qDebug() << 411 << json_entries.count();
           Entry * entry;
           for (int n = 0; n < json_entries.size(); n++){
               {
                   QJsonObject list_entry;
                   list_entry = json_entries[n].toObject();
                   //entry = biblioModel->contains_by_size(list_entry["size"].toInt())->first() ;

                   QList<Entry*> entries = get_entries_by_size(list_entry["size"].toInt());
                   entry = entries.first();
                   add_entry_to_list(list, entry);
               }
            }
       }
    }
   link_top_and_bottom_entries_from_size();
   link_top_and_bottom_entries_from_filename();
   //bottomTableWidget2->setVisible(true);
   //place_entries_with_shared_filename_on_table();
   //place_entries_with_shared_keys_on_table();
}
