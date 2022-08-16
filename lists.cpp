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



void Widget::select_new_list(QListWidgetItem * item){
    save_current_list();
    List* list;
    /* item is from List-of-lists listwidget */
    list = m_Lists_map[item->text()];
    display_list(list);
    m_current_list = list;
}


void Widget::save_current_list(){
    if (!m_current_list) {return;}
    qDebug() << 136 << "I will try to save the old list";
    for (int row = 0; row < listWidget->count(); row++){
        QString string = listWidget->item(row)->text();
        qDebug() << 127 << string << "Saving current list. Not implemented, though !!!";
    }
}
void Widget::display_list(List* list){
    listWidget->clear();
    foreach (Entry * entry, *list->get_entries()){
        listWidget->addItem(entry->get_author());
    }
}
void Widget::new_list( ){
    QString list_name = m_new_list_name_widget->text();
    List* list = new List(list_name);
    if (m_Lists_map.contains(list_name)){
        qDebug() << "Attempt to create a new list when it already exists.";
    }
    m_Lists.append(list);
    m_Lists_map[list_name] = list;
    listNamesWidget->addItem(list_name);
}
void Widget::new_list(QString list_name = QString() ){
    List* list = new List(list_name);
    if (m_Lists_map.contains(list_name)){
        qDebug() << "Attempt to create a new list when it already exists.";
    }
    m_Lists.append(list);
    m_Lists_map[list_name] = list;
    listNamesWidget->addItem(list_name);

}
void Widget::add_entry_to_list( ){
    // This is from middle widget
    List* list = m_current_list;
    if (!list) {return;}
    list->add_entry(m_entry_in_middle_table);
    qDebug() << 294 << "author" << m_entry_in_middle_table->get_author();
    display_list(list);
    register_biblioentry_by_key_name_and_size(m_entry_in_middle_table);
}
void Widget::add_entry_to_list(List* list, Entry* entry ){
    // This is from middle widget
    if (!list) {return;}
    list->add_entry(entry);
    display_list(list);
    //add_entry_to_bibmodel(m_entry_in_middle_table);
}

void Widget::change_location_of_listsInfo(){
    QString filename= QFileDialog::getSaveFileName(this, "Choose File", QDir::home().absolutePath(), "JSON files (*.json)");
    if(filename.isEmpty())
            return;
    if (! filename.endsWith(".json")){
        filename += ".json";
    }
    QFile fileIn(filename);
    int slash_loc = filename.lastIndexOf("/") + 1;
    QString foldername = filename.left(slash_loc);
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
            return;
    m_settings.setValue("listsfilename", filename);
    m_settings.setValue("listsfoldername", foldername);
    qDebug() << 194 << filename << foldername;
}
