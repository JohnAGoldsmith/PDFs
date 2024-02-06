#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractTableModel>
#include "widget.h"
#include "file.h"

class FilesModel: public QAbstractTableModel{
    Q_OBJECT
    Widget *                  m_parent;
    QList<File*>              m_files;
    QMap<QString, File*>      m_data_by_filename_stem;
    QMultiMap<QString, File*> m_data_by_filename_stem_multiple;
    QMap<int, File*>          m_data_by_size;
    QMultiMap<int, File*>     m_data_by_size_multiple;
    QMap<QString, File*>      m_data_by_filenamefull;

    My_filename_SortFilterProxyModel *   m_proxyModel;

public:
    FilesModel ( Widget *parent );
    ~FilesModel();

    void           addFile(File*);


    int            rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QStringList    get_bibliography_labels() const  { return   m_parent->get_bibliography_labels();}
    bool           setData(const QModelIndex &index, const QVariant & value, int role) override;
    QVariant       headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
//    void           setSeletedFile(File * file) {m_selected_file = file;}
    My_filename_SortFilterProxyModel* getProxyModel() {return m_proxyModel;}
    int            number_of_files(){return m_files.count();}
    QList<File*> * get_files(){return & m_files;}
    File*          get_file_with_filename(QString stem) {return m_data_by_filename_stem.value(stem);}
    QList<File*>   get_files_with_filename(QString stem) {return m_data_by_filename_stem_multiple.values(stem);}
    File*          get_file_by_full_filename(QString filename) {return m_data_by_filenamefull.value(filename);}
    bool            contains_entry_with_filename(QString stem) {return m_data_by_filename_stem.contains(stem);}
    bool            contains_multiple_entries_with_filename(QString stem){return m_data_by_filename_stem_multiple.contains(stem);}
    bool            contains(File * file) { return m_files.contains(file);}

    void            register_file(File*);
    void            register_file_by_size(File*);
    void            register_file_by_filename_full(File*);

    int             get_count_of_multiply_used_sizes() {return m_data_by_size_multiple.keys().count();}
    File*          get_file_by_size(int size) {return m_data_by_size[size];}
    QList<int>      get_list_of_sizes_used() {return m_data_by_size.keys();}
    QList<File*>   get_multiple_files_from_size(int size) {return m_data_by_size_multiple.values(size);}
    bool            if_size_occurs_multiply(int size) {return m_data_by_size_multiple.contains(size);}
    bool            contains_size(int this_size);

    int             get_count_of_multiply_used_filename_stems() {return m_data_by_filename_stem_multiple.keys().count();}
    File*          get_file_by_filename_stem(QString stem) {return m_data_by_filename_stem[stem];}
    QList<QString>  get_list_of_filename_stems_used() {return m_data_by_filename_stem.keys();}
    QList<File*>   get_multiple_files_from_filename_stem(QString stem) {return m_data_by_filename_stem_multiple.values(stem);}
    bool            if_filename_stem_occurs_multiply(QString stem) {return m_data_by_filename_stem_multiple.contains(stem);}
    bool            contains_filename_stem(QString stem);

    void            search_folders_for_pdfs(QString root_folder);

};

#endif // FILESMODEL_H
