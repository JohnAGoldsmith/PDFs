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
#include "BiblioTableModel.h"

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
void Widget::read_init_file( ){
    QJsonObject json_init;
    QString filename = "\\pdf_manager_ini.json";
//    if (m_init_folder filename.isEmpty()){
//                filename= QFileDialog::getOpenFileName(this, "Choose File", m_json_folder, "JSON files (*.json)");
//                if(filename.isEmpty())
//                    return;
//            }
    QFile fileIn(filename);
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
         return;
    //int slash_loc = filename.lastIndexOf("/") + 1;
    //foldername = filename.left(slash_loc);
    //m_json_folder= foldername;
    QByteArray bytearray = fileIn.readAll();
    QJsonParseError parseError;
    QJsonDocument json_doc =  QJsonDocument::fromJson(bytearray, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
         qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    } else {
         json_doc.toJson(QJsonDocument::Compact);
    }

    //m_settings.setValue("jsonfilename", filename);
    //m_settings.setValue("jsonfoldername", foldername);
    fileIn.close();
    if(!json_doc.isObject()){
            qDebug() << "JSON doc root is not an object.";
    }


}
void set_qtableview_widths(QTableView * TV){
    TV->setColumnWidth(0,300);
    TV->setColumnWidth(1,400);
    TV->setColumnWidth(2,100);
    TV->setColumnWidth(3,400);
    TV->setColumnWidth(4,300);
    TV->setColumnWidth(5,150);
    TV->setColumnWidth(6,100);
    TV->setColumnWidth(7,400);
    TV->setColumnWidth(8,400);
}
void Widget::read_JSON_file_new(QString filename){
    if (m_biblioModel) { delete m_biblioModel; }
    m_biblioModel =  m_biblioModel = new BiblioTableModel(this);
    int version(-1);
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

    if(!json_doc.isArray()){
       qDebug() << "JSON doc root is not an array.";
    }
    QJsonObject  json_settings;
    json_settings = json_doc[0].toObject();
    if (json_settings.contains("m_directory_view_root") && json_settings["m_directory_view_root"].isString() ){
        m_directory_view_root = json_settings["m_directory_view_root"].toString();
        m_file_system_model->setRootPath(m_directory_view_root);
    }
    if (json_settings.contains("version") && json_settings["m_directory_view_root"].isString() ){
        version = json_settings["version"].toInt();
    }  else{
        version = 0;
    }
    switch (version){
        case 0:{
            json_bibliography = json_doc[2].toObject();
        }
        default: {
            json_bibliography = json_doc[1].toObject();
        }
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
                if (entry_internal_key == "filenamestem" || entry_internal_key == "folder") { // to do todo  We probably don't want to ignore this in the future
                    continue;
                }
                if (entry_internal_key == "size"){
                    entry->set_size(j_this_entry["size"].toInt());
                } else {
                    QString value = j_this_entry[entry_internal_key].toString();
                    entry->set_info(entry_internal_key, value);
                }
            }
            m_biblioModel->add_entry(entry);
        }
    }
    m_biblioModel->register_all_entries();
    m_biblioModel->m_proxyModel = new MySortFilterProxyModel () ;
    m_biblioModel->m_proxyModel->setSourceModel( m_biblioModel );

    m_topTableView->setModel( m_biblioModel->m_proxyModel );
    set_qtableview_widths(m_topTableView);

    if (m_onboard_pdf_model){
         link_biblio_entries_and_onboard_entries_from_size();
    }
}
