#include "file_model.h"
#include "file.h"

FileModel::FileModel() {

}

FileModel::FileModel(File * file, Widget* widget){
    m_file = file;
    m_parent = widget;
}

FileModel::~FileModel(){

}
int FileModel::rowCount(const QModelIndex &parent) const {
    return  6;
}
int FileModel::columnCount(const QModelIndex &parent) const {
    return 2;
}
QVariant FileModel::data(const QModelIndex & index, int role) const{
    qDebug() << 22 << index.isValid() << index.row() << index.column()<< role;
    if (index.isValid() && role == 0) {
        qDebug() << 23 << index.column();
        switch (index.column()){
        case 0:
            switch (index.row()){
            case 0:
                return QVariant("filename");
            case 1:
                return QVariant("folder");
            case 2:
                return QVariant("size");
            case 3:
                return QVariant("date created");
            case 4:
                return QVariant("date last read");
            case 5:
                return QVariant("date last modified");
            }
        case 1:
            switch (index.row()){
                case 0:
                    return QVariant(m_file->get_filename());
                case 1:
                    return QVariant(m_file->get_folder());
                case 2:
                    return QVariant(m_file->get_size());
                case 3:
                    return QVariant(m_file->get_date_created());
                case 4:
                    return QVariant(m_file->get_date_last_read());
                case 5:
                    return QVariant (m_file->get_date_last_modified());
            }
        }
        return QVariant();
    }
}
void FileModel::announce_data_change(){
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(5,2);
    emit dataChanged(topLeft, topLeft);
}
