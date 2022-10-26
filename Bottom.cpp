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
#include <QHeaderView>
#include <EGL/egl.h>
class List;




/*              BOTTOM VIEW                 */
// This should really be a function of the lower widget, if I subclass it.
// Currently this is only used by a function no longer used, one that reads the old format. It should be removed.
/*
void::Widget::mark_file_as_indexed(QString filename){
    if (! filename.endsWith(".pdf")){
        filename += ".pdf";
    }
    for (int rowno = 0; rowno < bottomTableWidget->rowCount(); rowno++){
        QString this_file_name = bottomTableWidget->item(rowno,0)->text() +
                bottomTableWidget->item(rowno,1)->text();
        if (filename == this_file_name){
            QTableWidgetItem * item = bottomTableWidget->item(rowno,2);
            item->setForeground(QBrush(QColor(0,255, 255)));
        }
    }
}
*/

/*
void Widget::on_bottom_table_widget_clicked(const QModelIndex& index){
    //convert to Entry*, then pass a signal
    int row = index.row();
    QString  stem =   bottomTableWidget->item(row,1)->text();
    QString filename = bottomTableWidget->item(row,2)->text() + "/" + stem;
    Entry * entry = m_files_onboard_by_filenamefull[filename];
    //m_selected_row_in_bottom_table = index.row();
    //put_file_info_on_middle_table_widget(m_selected_row_in_bottom_table); // this should send the entry, not the row, no?
    put_file_info_on_middle_table_widget(entry);
}
*/
void Widget::on_bottom_table_view_clicked(const QModelIndex& index){
    if (onboard_pdf_model->number_of_entries() < 1) {return;}
    int row =  onboard_pdf_model->getProxyModel()->mapToSource( index  ).row() ;
    if (row < 0) {return;}
    QString  stem =   onboard_pdf_model->index(row,1).data().toString();
    QString filename = onboard_pdf_model->index(row,2).data().toString() + "/" + stem;
    Entry * entry = m_files_onboard_by_filenamefull[filename];
    if (!m_selected_entry_model){
        m_selected_entry_model = new EntryModel(entry, m_bibliography_labels);
    } else {
        m_selected_entry_model->change_entry(entry);
        m_selected_onboard_entry = entry;
    }
}

void Entry::remove_bottom_view_links(){
    m_bottom_view_size_item = nullptr;
    m_bottom_view_filename_item = nullptr;
    }
void Widget::search_folders_for_pdf()
{
    // For each entry in the top model, remove the link to items in the bottom widget:
    delete onboard_pdf_model;    
    onboard_pdf_model = new EntriesModel(this);
    m_bottomTableView->setModel(onboard_pdf_model->getProxyModel());

    m_files_onboard_by_filenamefull.clear();
    m_files_onboard_by_filenamestem.clear();
    m_files_onboard_by_size.clear();
    QString targetStr = ".pdf"; // What we search for
    QString filenameStem,  folder, filenamefull;
    int size;
    QFileInfoList hitList; // Container for matches
    QDirIterator it(m_root_folder, QDirIterator::Subdirectories);
    while (it.hasNext()) {
            QString filename = it.next();
            QFileInfo file(filename);
            if (file.isDir()) { // Check if it's a dir
                continue;
            }
            if (file.fileName().endsWith(targetStr, Qt::CaseInsensitive)) {
                if (file.size()== 0){continue;}
                hitList.append(file);
            }
        }

     foreach (QFileInfo hit, hitList) {
        Entry * entry = new Entry();
        filenameStem = hit.fileName();
        folder = hit.dir().absolutePath();
        filenamefull = folder + "/" + filenameStem;
        size = hit.size();
        if (size == 0){
            continue;
        }
        entry->set_folder(folder);
        entry->set_filenameStem(filenameStem);
        entry->set_info("date", hit.lastModified().date().toString("yyyy MM dd"));
        entry->set_info("lastread", hit.lastRead().date().toString("yyyy MM dd"));
        entry->set_size(size);
        m_files_onboard_by_filenamestem.insert(filenameStem, entry);
        m_files_onboard_by_filenamefull.insert(filenamefull, entry);
        m_files_onboard_by_size.insert(size, entry);
        onboard_pdf_model->addEntry(entry);  // this should replace a huge amount of other code above!
    }
    m_bottomTableView->resizeColumnsToContents();
    m_bottomTableView->sortByColumn(0);
    link_top_and_bottom_entries_from_size(); // TODO: clear out any previous linkings before doing this;

    // link_top_and_bottom_entries_from_filename(); // TODO: clear out any previous linkings before doing this;
}
void Widget::set_filename_item_bottom_widget(int row, QString new_name){
    QTableWidgetItem * item = m_bottom_table_widget->item(row,1);
}

void Widget::delete_selected_files(){
    // TODO why the rowcount -1 ? Check for presence of entries in the widget.
    for(int row = 0; row < m_bottom_table_widget->rowCount()-1; row++){
        //qDebug() << 1287 << "row" << row;
        if (m_bottom_table_widget->item(row,0)->checkState()==Qt::Checked){
            //qDebug() << 1289;
            QString filename = m_bottom_table_widget->item(row,2)->text() + "/" + m_bottom_table_widget->item(row,1)->text();
            QFile file(filename);
            //qDebug() << 1292;
            file.remove();
        }
        //qDebug() << 1293 << "row" << row;
    }
    m_bottom_table_widget->clear();
    search_folders_for_pdf();

}
void Widget::show_files_with_same_size(){
    QStringList horizontal_headers;
    horizontal_headers << "Filename " << "Folder" << "Size";
    QList<int> sizes = m_files_onboard_by_size.uniqueKeys();
    if (same_size_files_model) { delete same_size_files_model;}
    same_size_files_model = new EntriesModel(this);
    m_bottomTableView->setModel(same_size_files_model);


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
            foreach(Entry* entry, m_files_onboard_by_size.values(size)){
                // --------------------------------------
                item0 = new QTableWidgetItem();
                item0->setCheckState(Qt::Unchecked);
                item1 = new QTableWidgetItem(entry->get_filenamestem());
                item2 = new QTableWidgetItem(entry->get_folder());
                item3 = new QTableWidgetItem();
                item3->setData(Qt::DisplayRole, size);
                item4 = new QTableWidgetItem();
                item4->setData(Qt::DisplayRole, entry->get_info("date"));
                item5 = new QTableWidgetItem();
                item5->setData(Qt::DisplayRole, entry->get_info("lastread"));
                entry->set_bottom_view_filename_item(item1);
                entry->set_bottom_view_size_item(item3);
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
                same_size_files_model->addEntry(entry);
            }
        }
    }
}
