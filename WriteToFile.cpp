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
QString remove_prefix_and_remove_spaces(QString & string);

void Widget::read_ToK_from_json(QString filename){

    QFile fileIn(filename);
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QByteArray bytearray = fileIn.readAll();
    QJsonParseError parseError;
    QJsonDocument json_doc =  QJsonDocument::fromJson(bytearray, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    } else {
            json_doc.toJson(QJsonDocument::Compact);
    }
    if(!json_doc.isObject()){
            qDebug() << "JSON doc root is not an object.";
    }
    QJsonArray json_ToK;
    json_ToK =  json_doc["ToK"].toArray();

    foreach (QJsonValue value, json_ToK){
        QJsonObject this_object = value.toObject();
        QString prefix = this_object["prefix"].toString();
        QString string =  this_object["string"].toString();
        //qDebug() << "";
        //qDebug() << 77 << "reading json"<< this_object["prefix"].toString() << this_object["string"].toString();
        m_ToK_model->addItem(prefix, string);


    }

}
void Widget::write_ToK_to_file(){
    QString outfile_name = "pdf_manager_tok_init.json";
    bool key_only_flag = false;
    QList<Prefix_String*> lines = m_ToK_model->output(key_only_flag);  // key_only means that each line has only the lowest (last) digit of its prefix
    QJsonObject json_top;
    QJsonArray json_ToK; // tree of knowledge
    foreach (Prefix_String* PS,  lines){
        QJsonObject json_line;
        json_line["prefix"] = QJsonValue(PS->m_prefix);
        json_line["string"] = QJsonValue(PS->m_string);
        json_ToK.append(json_line);
    }
    json_top["ToK"] = json_ToK;
    QFile saveFile(outfile_name);
    if (!saveFile.open(QIODevice::WriteOnly)) {
         qWarning("Couldn't open save file.");
         return;
    }
    qDebug() << 610 << outfile_name;
    QJsonDocument doc(json_top);
    saveFile.write(doc.toJson());
    qDebug() << "finished writing tok";

}
/*           WRITE TO FILE              */
void write_list_to_json(List* list, QJsonArray & json_array ){
    QJsonObject this_list;
    this_list["list_name"] = list->get_name();
    QJsonArray this_list_entries;
    for (int n=0; n < list->get_count_of_entries(); n++){
        QJsonObject this_entry;
        Entry* entry = list->get_entry(n);
        this_entry["key"]= entry->get_key();
        this_entry["title"]= entry->get_title();
        this_entry["author"]= entry->get_author();
        this_entry["year"]= entry->get_year();
        this_entry["size"]= entry->get_size();
        this_list_entries.append(this_entry);
    }
    this_list["entries"] = this_list_entries;
    json_array.append(this_list);

}
/*
void Widget::write_lists_to_json(QJsonArray & json_array){
    foreach (List* list, m_Lists){
        write_list_to_json (list,json_array);
    }
}
*/
void Widget::write_bibliography(){
    write_bibliography_to_json();
    write_bibliography_to_bibtex();
    write_ToK_to_file();
}
void Widget::write_bibliography_to_json( ){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyy_MM_dd_hh:mm:ss");
    qDebug() << 487 << formattedTime;
    QJsonArray json_top;
    QJsonObject json_settings;
    json_settings["m_root_folder"] = m_root_folder;
    json_settings["m_json_folder"] = m_json_folder;
    json_settings["m_directory_view_root"] = m_directory_view_root;
    json_top.append(json_settings);

    QJsonObject json_object_for_biblio;
    foreach (Entry* entry, m_biblioModel->get_entries()){
        write_bibentry_to_json(entry, json_object_for_biblio ); //TODO  HERE IS WHERE I stopped. have to iterate over the right key.
    }
    json_top.append(json_object_for_biblio);

    QStringList filenames;
    filenames << "bibliography.json" << formattedTime + "bibliography.json";
    foreach (QString this_filename, filenames){
        QString fullfilename(m_json_folder + this_filename );
        QFile saveFile(fullfilename);
        if (!saveFile.open(QIODevice::WriteOnly)) {
             qWarning("Couldn't open save file.");
             return;
        }
        qDebug() << 610 << fullfilename;
        QJsonDocument doc(json_top);
        saveFile.write(doc.toJson());
    }
}
void Widget::write_bibliography_to_bibtex(){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyy_MM_dd_hh:mm:ss");
    QString filename = m_json_folder + "this_bibliography.bibtex";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
         qWarning("Couldn't open save file.");
         return;
    }
    QTextStream stream(&file);
    foreach (Entry* entry, m_biblioModel->get_entries()){
        entry -> write_bibentry_to_bibtex(  stream , m_bibliography_labels); //TODO  HERE IS WHERE I stopped. have to iterate over the right key.
    }
    qDebug() << 770 << filename;
    file.close();
}

void Entry::write_bibentry_to_bibtex( QTextStream & stream, QStringList & bibliography_labels){

    QStringList labels, values;
    QString mytype, author, title, year, key, size;

    stream << QString("@");

    if (info["type"].length() == 0) {
        mytype = "article";
    } else {
        mytype = info["type"];
    }
    stream << mytype << "{" << info["key"] << "{ \n";

    if (info["author"].length() == 0) {
        author = "author_unknown";
    } else {
        author = get_author();
    }
    labels << "author";
    values << author;

    if (info["title"].length() == 0) {
        title = "no title";
    } else {
        title = get_title();
    }
    labels << "title";
    values << title;

    if (info["year"].length() == 0) {
        year = "no year";
    } else {
        year = get_year();
    }
    labels << "year";
    values << year;

    if (info["key"].length() == 0) {
        key = "no key";
    } else {
        key = get_key();
    }
    labels << "key";
    values << key;

    if (get_size() > 0) {
        size = QString::number(get_size());
    } else{
        size =  "0";
    }
    labels << "size";
    values << size;

    QStringList first_labels;
    first_labels << "type" << "author" << "title" << "year" << "key" << "size";

    for (int i = 0; i < bibliography_labels.length(); i++){
        QString label = bibliography_labels[i];
        if (first_labels.contains(label)){continue;}
        if (get_info(label).length() > 0){
            labels <<  label ;
            values <<  get_info(label)  ;
        }
    }
    for (int i = 0; i < labels.length(); i++){
        stream << "    " <<  "\"" + labels[i] + "\""    <<    " =  \"" +  values[i] + "\"";
        if (i < labels.length() - 1){
            stream << ",";
        }
        stream << "\n";
    }
    stream << "}\n";
}

void Widget::write_bibentry_to_json(Entry * entry, QJsonObject & total_json_object)
{    QString key;
    QJsonObject this_json_entry;
    if (entry->get_size() > 0){
        this_json_entry["size"] = entry->get_size();
    }
    foreach (QString label, m_bibliography_labels){
        if (label == "date_entry_created" && entry->get_info(label).isEmpty()){\
            QDateTime datetime = datetime.currentDateTime();
            QString format = "ddd MMMM d yyyy hh:mm";
            entry->set_info(label, datetime.toString(format));
        }
        if (entry->get_info(label).length() > 0){
            this_json_entry[label] = entry->get_info(label);
        }
        if (label=="key"){
            key = entry->get_info("key");
        }
    }
    int fileno = 0;
    QString file = "possible_file::" + QString::number(fileno);
    while(entry->contains_info(file)){
        this_json_entry[file] = entry->get_info(file);
        qDebug() << 589 << entry->get_info(file);
        fileno++;
        file = "possible_file::" + QString::number(fileno);
    }
    total_json_object[key] = this_json_entry;
}
