#ifndef ENTRIESMODEL_H
#define ENTRIESMODEL_H

#include <QAbstractTableModel>
#include "widget.h"


class EntriesModel: public QAbstractTableModel{
    Q_OBJECT
    Widget *                   m_parent;
    Entry*                     m_selected_entry; // ?? is this needed?

    QList<Entry*>              m_entries;

    QMap<QString, Entry*>      m_data_by_filename_stem;
    QMultiMap<QString, Entry*> m_data_by_filename_stem_multiple;

    QMap<int, Entry*>           m_data_by_size;
    QMultiMap<int, Entry*>      m_data_by_size_multiple;


    QMap<QString, Entry*>       m_data_by_filenamefull;




public:
    EntriesModel ( Widget *parent );
    ~EntriesModel();
    My_onboard_SortFilterProxyModel *   m_proxyModel;

    //index() override;
    //parent() override;
    int            rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QStringList    get_bibliography_labels() const  { return   m_parent->get_bibliography_labels();}
    bool           setData(const QModelIndex &index, const QVariant & value, int role) override;
    void           addEntry(Entry*);
    QVariant       headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void           setSeletedEntry(Entry* entry) {m_selected_entry = entry;}
    My_onboard_SortFilterProxyModel* getProxyModel() {return m_proxyModel;}
    int            number_of_entries(){return m_entries.count();}
    QList<Entry*> * get_entries(){return & m_entries;}
    Entry*          get_entry_with_filenamestem(QString stem) {return m_data_by_filename_stem.value(stem);}
    QList<Entry*>   get_entries_with_filenamestem(QString stem) {return m_data_by_filename_stem_multiple.values(stem);}
    Entry*          get_entry_by_full_filename(QString filename) {return m_data_by_filenamefull.value(filename);}
    bool            contains_entry_with_filenamestem(QString stem) {return m_data_by_filename_stem.contains(stem);}
    bool            contains_multiple_entries_with_filenamestem(QString stem){return m_data_by_filename_stem_multiple.contains(stem);}
    bool            contains(Entry * entry) { return m_entries.contains(entry);}

    void            register_entry(Entry*);
    void            register_entry_by_size(Entry*);
    void            register_entry_by_filename_full(Entry*);

    int             get_count_of_multiply_used_sizes() {return m_data_by_size_multiple.keys().count();}
    Entry*          get_entry_by_size(int size) {return m_data_by_size[size];}
    QList<int>      get_list_of_sizes_used() {return m_data_by_size.keys();}
    QList<Entry*>   get_multiple_entries_from_size(int size) {return m_data_by_size_multiple.values(size);}
    bool            if_size_occurs_multiply(int size) {return m_data_by_size_multiple.contains(size);}
    bool            contains_size(int this_size);

    int             get_count_of_multiply_used_filename_stems() {return m_data_by_filename_stem_multiple.keys().count();}
    Entry*          get_entry_by_filename_stem(QString stem) {return m_data_by_filename_stem[stem];}
    QList<QString>  get_list_of_filename_stems_used() {return m_data_by_filename_stem.keys();}
    QList<Entry*>   get_multiple_entries_from_filename_stem(QString stem) {return m_data_by_filename_stem_multiple.values(stem);}
    bool            if_filename_stem_occurs_multiply(QString stem) {return m_data_by_filename_stem_multiple.contains(stem);}
    bool            contains_filename_stem(QString stem);

};

#endif // ENTRIESMODEL_H
