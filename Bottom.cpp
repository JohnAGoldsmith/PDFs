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
#include <QAbstractScrollArea>
#include <QHeaderView>
#include <EGL/egl.h>
#include <QTableView>
#include "FilesModel.h"
#include "Entry.h"
#include "file.h"
#include "EntryModel.h"
#include "file_model.h"
//class List;




/*              BOTTOM VIEW                 */
// This should really be a function of the lower widget, if I subclass it.
// Currently this is only used by a function no longer used, one that reads the old format. It should be removed.






















void Widget::set_filename_item_bottom_widget(int row, QString new_name){
    QTableWidgetItem * item = m_bottom_table_widget->item(row,1);
}
void Widget::delete_selected_files(){
    // TODO why the rowcount -1 ? Check for presence of entries in the widget.
    for(int row = 0; row < m_bottom_table_widget->rowCount()-1; row++){
        if (m_bottom_table_widget->item(row,0)->checkState()==Qt::Checked){
            QString filename = m_bottom_table_widget->item(row,2)->text() + "/" + m_bottom_table_widget->item(row,1)->text();
            QFile::moveToTrash(filename);
            //QFile file(filename);
            //file.remove();
        }
    }
    m_bottom_table_widget->clear();
    search_folders_for_pdf();

    QModelIndexList selection = m_bottom_tableView->selectionModel()->selectedRows();

    // Multiple rows can be selected
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        qDebug() << index.row();
    }
}


/* This is not needed; EntryMatchModel will simply display itself on a TableView */
/*
void Widget::show_files_with_same_size(){
    QStringList horizontal_headers;
    horizontal_headers << "Filename " << "Folder" << "Size";
    QList<int> sizes = m_files_onboard_by_size.uniqueKeys();
    if (m_same_size_files_model) { delete m_same_size_files_model;}
    m_same_size_files_model = new FilesModel(this);
    m_bottom_tableView->setModel(m_same_size_files_model);


    int row = 0;
    int start_row = 0;
    int stop_row = 0;
    QList<QString> filenames;
    QTableWidgetItem* item0, *item1, *item2, *item3, *item4, *item5;
    int color = 0;
    foreach (int size, sizes){
        if (m_files_onboard_by_size.count(size) > 1){
            QTableWidgetItem * item1, * item2, * item3, *item0;
            start_row = row;
            foreach(File* file, m_files_onboard_by_size.values(size)){
                // --------------------------------------
                item0 = new QTableWidgetItem();
                item0->setCheckState(Qt::Unchecked);
                item1 = new QTableWidgetItem(file->get_filename());
                item2 = new QTableWidgetItem(file->get_folder());
                item3 = new QTableWidgetItem();
                item3->setData(Qt::DisplayRole, size);
                item4 = new QTableWidgetItem();
                item4->setData(Qt::DisplayRole, file->get_info("date"));
                item5 = new QTableWidgetItem();
                item5->setData(Qt::DisplayRole, file->get_info("lastread"));
                //entry->set_bottom_view_filename_item(item1);
                //entry->set_bottom_view_size_item(item3);
                //---------------------------------------
                if (row == start_row ){
                   if (color == 0){
                       color = 1;}
                   else{ color = 0;}
                }
                if (color == 0){
                    item1->setForeground(QColorConstants::Red);
                    entry->set_temporary_color(QColorConstants::Red);
                }else{
                    item1->setForeground(QColorConstants::Blue);
                    entry->set_temporary_color(QColorConstants::Blue);
                }
                stop_row = row;
                row++;
                m_same_size_files_model->addFile(file);
            }
        }
    }
}
*/
