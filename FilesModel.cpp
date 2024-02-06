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
#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <EGL/egl.h>
#include "FilesModel.h"
#include "Entry.h"
//class List;





FilesModel::FilesModel(Widget * main_widget){
    m_parent = main_widget;
    //m_entry = main_widget->m_entry_in_bottom_table;
    m_proxyModel = new My_filename_SortFilterProxyModel();
    m_proxyModel->setSourceModel(this);
}
FilesModel::~FilesModel(){

}

int test_for_pattern (QString str){
    if (str.length() < 4){
        return 0;
    }
    int depth = 0;
    while (str.length() >= 2){
        if (str[0].isLetterOrNumber() && str[1] == ' '){
            depth += 1;
            str = str.sliced(2);
        } else{
            break;
        }
    }
    if (depth >= 2) {return depth;}
    return 0;
}
QString get_pattern(QString str, int depth){
    return str.left(depth*2 - 1);
}
QString get_remainder(QString str, int depth){
    return str.sliced(depth*2).toLower();
}
/* The following could be done differently: instead of using a
 * SortFilterProxyModel, we could have changed the sort() function for
 * the EntriesModel for a particular column number.
 */
bool My_filename_SortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{
    QString left_name = sourceModel()->data(left).toString();
    QString right_name = sourceModel()->data(right).toString();
    int left_depth = test_for_pattern(left_name);
    int right_depth = test_for_pattern(right_name);
    QString left_pattern = get_pattern(left_name, left_depth);
    QString right_pattern = get_pattern (right_name, right_depth);
    if (left_depth == 0 && right_depth == 0){
        if (left_name.toLower() < right_name.toLower()){
            return true;
        }
        return false;
    }
    if (left_depth == 0 && right_depth > 0){
        return false;
    }
    if(left_depth > 0 && right_depth == 0){
        return true;
    }

    if (left_pattern < right_pattern){
        return true;
    }
    if (left_pattern == right_pattern){
        if (get_remainder(left_name, left_depth) < get_remainder(right_name, right_depth) ){
            return true;
        }
    }
    return false;
}

bool FilesModel::contains_size(int this_size){
    if (m_data_by_size.contains(this_size)) {
        return true;
    }
    return false;
}
bool FilesModel::contains_filename_stem(QString stem){
    if (m_data_by_filename_stem.contains(stem)){
        return true;
    }
    return false;
}
QVariant FilesModel::headerData(int section, Qt::Orientation orientation, int role) const{
  QStringList horizontal_headers;
  horizontal_headers << " " << "File name" << "Folder" << "Size" << "Date created" << "Date accessed";
  if (role == Qt::DisplayRole)
  {
    if (orientation == Qt::Horizontal) {
        if (section >= 0 && section < horizontal_headers.size()){
            return horizontal_headers[section];
        } else {
          return QString("test");
        }
    }
  }
  return QVariant();

};
void FilesModel::addFile(File * file){
    m_files.append(file);
    register_file(file);
}
void FilesModel::register_file(File* file){
    // qDebug() << file->get_filename() << 110;
    register_file_by_size(file);
    register_file_by_filename_full(file);
}
void FilesModel::register_file_by_filename_full(File* file){
    //QString temp = file->get_filenamefull();
    //qDebug() << file->get_filename() << 117;
    m_data_by_filenamefull[file->get_folder() + "/"   + file->get_filename()] = file;
}
void FilesModel::register_file_by_size(File* file){
    int size = file->get_size();
    if (size == 0) {return;}
    if (m_data_by_size_multiple.contains(size)){
        m_data_by_size_multiple.insert(size, file);
    } else {
        if (m_data_by_size.contains(size) ) {             
              m_data_by_size_multiple.insert(size, m_data_by_size[size]);
              m_data_by_size_multiple.insert(size, file);
            }
           else {
              m_data_by_size.insert(size, file);
           }
    }
}
int FilesModel::rowCount(const QModelIndex &index ) const
{
//   qDebug() << 66 << m_entries.count();
   Q_UNUSED(index);
   return  m_files.count();
}

int FilesModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 6;
}
bool FilesModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row =  m_proxyModel->mapToSource(index).row();
        File* file = m_files.value(row);
        if (index.column() == 1){
            switch (index.row()) {
                case 0:{
                    file->set_filename_stem(value.toString());
                    break;
                }
                case 1:{
                    file->set_folder(value.toString());
                    break;
                }
                case 3:{
                    file->set_author(value.toString());
                    break;
                }
                case 4:{
                    file->set_title(value.toString());
                    break;
                }
                case 5:{
                    file->set_year(value.toString());
                    break;
            }
            }

        }
    }
    return false;
}


QVariant FilesModel::data(const QModelIndex & index, int role )const
{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    if (m_files.count() == 0){
        return QVariant();
    }
    File* file = m_files.at(row);
    if (!file) return QVariant();

    if (role == 0) {

        switch (index.column()){
        case 0:
            return QVariant (index.row()+1);
         case 1:
            return QVariant(file->get_filename() );
            break;
        case 2:
           return QVariant(file->get_folder());
           break;
        case 3:
           return QVariant(file->get_size());
           break;
        case 4:
           return QVariant(file->get_date_created());
           break;

        default:
            return QVariant(file->get_date_last_read());
            break;
        }
    }

    if (role == Qt::BackgroundRole && file->if_linked_to_biblio_entry()){
            QColor col1(0,255,255);
            QVariant var2 = col1;
            return var2;
    }
//    if (role == 256){
//        return  QVariant::fromValue(static_cast<void*>(entry));
//    }
     // end of role 0, which is displayed material

    return QVariant();
    }

void FilesModel::search_folders_for_pdfs(QString root_folder){
    QString targetStr = ".pdf"; // What we search for
    QFileInfoList hitList; // Container for matches
    QDirIterator it(root_folder, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filename = it.next();
        QFileInfo qfile(filename);
        if (qfile.isDir()) { // Check if it's a dir
            continue;
        }
        if (qfile.fileName().endsWith(targetStr, Qt::CaseInsensitive)) {
            if (qfile.size()== 0){continue;}
            hitList.append(qfile);
        }
    }
    foreach (QFileInfo hit, hitList) {
        File * file = new File(hit);
        addFile(file);
    }
    QModelIndex topLeft = createIndex(0,0);
    emit dataChanged(topLeft, topLeft);
}


TableView_for_files::TableView_for_files(){
    setSortingEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    //m_bottomTableView->horizontalHeader()->setMaximumSectionSize(int);
}
