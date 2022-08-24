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
    QString  stem =   bottomTableWidget->item(row,1)->text();
    QString filename = bottomTableWidget->item(row,2)->text() + "/" + stem;
    qDebug() << filename;
    Entry * entry = m_files_onboard_by_filenamefull[filename];
    put_file_info_on_middle_table_widget(entry);
    put_file_info_on_popup_widget(entry);
}
/*
void Widget::on_bottom_table_widget_doubleClicked(int row, int column){
    Q_UNUSED(column);
    QTableWidgetItem  * itemfolder =  bottomTableWidget->item(row,2);
    QString foldername = itemfolder->data(Qt::DisplayRole).toString();
    QTableWidgetItem* itemfilename = bottomTableWidget->item(row,1);
    QString filename = itemfilename->data(Qt::DisplayRole).toString();
    QString completename = foldername + "/" +  filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(completename));
    Entry* entry = m_files_onboard_by_filenamefull[completename];
    m_entry_in_bottom_table = entry;

}
*/
void Entry::remove_bottom_view_links(){
    m_bottom_view_size_item = nullptr;
    m_bottom_view_filename_item = nullptr;
    }
void Widget::search_folders_for_pdf()
{
    // For each entry in the top model, remove the link to items in the bottom widget:

    foreach (QString key, biblioModel->get_keys() ) {
        biblioModel->get_entry_by_key(key)->remove_bottom_view_links();
        // replaces this:
        //m_data_by_key[key]->remove_bottom_view_links();
    }
    //bottomTableWidgetFunction = allOnboardFiles;
    bottomTableWidget->clear();
    bottomTableWidget->setColumnWidth(0,40);
    bottomTableWidget->setColumnWidth(1,500);
    bottomTableWidget->setColumnWidth(2,500);
    bottomTableWidget->setColumnWidth(3,100);
    bottomTableWidget->setColumnWidth(4,100);
    bottomTableWidget->setColumnWidth(5,100);
    m_files_onboard_by_filenamefull.clear();
    m_files_onboard_by_filenamestem.clear();
    m_files_onboard_by_size.clear();
    bottomTableWidget->clear();
    QString targetStr = ".pdf"; // What we search for
    QString filenameStem, filenameFull, folder;
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
    bottomTableWidget->setRowCount(hitList.count());
    bottomTableWidget->setColumnCount(6);
    int rowno = 0;
    int count = 0;
    QTableWidgetItem * item0, *item1,*item2,*item3, *item4, *item5;
    //int count1(0);
    //int count2(0);
    foreach (QFileInfo hit, hitList) {
        //qDebug() << count1++;
        filenameStem = hit.fileName();
        folder = hit.dir().absolutePath();
        size = hit.size();
        if (size == 0){
            continue;
        }
        filenameFull = folder + "/" + filenameStem;

        Entry * entry = new Entry();

        QString date (hit.lastModified().date().toString("yyyy MM dd"));
        QString lastread (hit.lastRead().date().toString("yyyy MM dd"));
        // Entry * entry = new Entry(hit.filename, hit.dir().absolutePath(),
        //                 folder + "/" + filenameStem, date, lastread, size);
        // we will get rid of:
        entry->set_folder(folder);
        entry->set_filenameStem(filenameStem);
        entry->set_filenameFull(filenameFull);
        entry->set_info("date", hit.lastModified().date().toString("yyyy MM dd"));
        entry->set_info("lastread", hit.lastRead().date().toString("yyyy MM dd"));
        //qDebug() << 1046 << hit.birthTime().toString("yyyy MM dd") << hit.absoluteFilePath() << hit.lastModified().date().toString();
        //qDebug() << 880 << entry->get_filenamefull();
        entry->set_size(size);
        m_files_onboard_by_filenamefull[filenameFull] = entry;
        m_files_onboard_by_filenamestem.insert(filenameStem, entry);
        if (m_files_onboard_by_size.contains(size)){
            count++;
        }
        m_files_onboard_by_size.insert(size, entry);

        //..........................................................
        // this will be removed:
        item0 = new QTableWidgetItem();
        item0->setCheckState(Qt::Unchecked);
        item1 = new QTableWidgetItem(filenameStem);
        item2 = new QTableWidgetItem(folder);
        item3 = new QTableWidgetItem();
        item3->setData(Qt::DisplayRole, size);
        item4 = new QTableWidgetItem();
        item4->setData(Qt::DisplayRole, entry->get_info("date"));
        item5 = new QTableWidgetItem();
        item5->setData(Qt::DisplayRole, entry->get_info("lastread"));
        entry->set_bottom_view_filename_item(item1);                          // is this needed?
        entry->set_bottom_view_size_item(item3);                              // ditto?
        bottomTableWidget->setItem(rowno,0,item0);
        bottomTableWidget->setItem(rowno,1,item1);
        bottomTableWidget->setItem(rowno,2,item2);
        bottomTableWidget->setItem(rowno,3,item3);
        bottomTableWidget->setItem(rowno,4,item4);
        bottomTableWidget->setItem(rowno,5,item5);
        //...............................................................


        onboard_pdf_model->addEntry(entry);  // this should replace a huge amount of other code above!
        rowno++;
    }
    bottomTableView->resizeColumnsToContents();
    bottomTableView->sortByColumn(1);
    //qDebug() << 164 << "Size of bottom view/rows" << bottomTableView->row
    link_top_and_bottom_entries_from_size(); // TODO: clear out any previous linkings before doing this;
    link_top_and_bottom_entries_from_filename(); // TODO: clear out any previous linkings before doing this;
}
void Widget::set_filename_item_bottom_widget(int row, QString new_name){
    QTableWidgetItem * item = bottomTableWidget->item(row,1);
}

void Widget::delete_selected_files(){
    // TODO why the rowcount -1 ? Check for presence of entries in the widget.
    for(int row = 0; row < bottomTableWidget->rowCount()-1; row++){
        //qDebug() << 1287 << "row" << row;
        if (bottomTableWidget->item(row,0)->checkState()==Qt::Checked){
            //qDebug() << 1289;
            QString filename = bottomTableWidget->item(row,2)->text() + "/" + bottomTableWidget->item(row,1)->text();
            QFile file(filename);
            //qDebug() << 1292;
            file.remove();
        }
        //qDebug() << 1293 << "row" << row;
    }
    bottomTableWidget->clear();
    search_folders_for_pdf();

}
void Widget::show_files_with_same_size(){
    //bottomTableWidgetFunction = sameSizeFiles;
    //bottomTableWidget->clear();

    QStringList horizontal_headers;
    horizontal_headers << "Filename " << "Folder" << "Size";
    QList<int> sizes = m_files_onboard_by_size.uniqueKeys();
    if (same_size_files_model) { delete same_size_files_model;}
    same_size_files_model = new EntriesModel(this);
    bottomTableView->setModel(same_size_files_model);


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
                //----------------------------------------
                bottomTableWidget->setItem(row,0,item0);
                bottomTableWidget->setItem(row,1,item1);
                bottomTableWidget->setItem(row,2,item2);
                bottomTableWidget->setItem(row,3,item3);
                bottomTableWidget->setItem(row,4,item4);
                bottomTableWidget->setItem(row, 5, item5);
                //----------------------------------------
                stop_row = row;
                row++;
                same_size_files_model->addEntry(entry);
            }
        }
    }
    //-------------------------------------------
    bottomTableWidget->setRowCount(row);
    bottomTableWidget->setColumnWidth(0,40);
    bottomTableWidget->setColumnWidth(1,500);
    bottomTableWidget->setColumnWidth(2,500);
    bottomTableWidget->setColumnWidth(3,100);
    bottomTableWidget->setColumnWidth(4,100);
    bottomTableWidget->setColumnWidth(5,100);
    //----------------------------------------------


}
