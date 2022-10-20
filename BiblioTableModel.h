#ifndef BIBLIOTABLEMODEL_H
#define BIBLIOTABLEMODEL_H
#include <QString>
#include <QAbstractTableModel>

class Entry;
class MySortFilterProxyModel;

class BiblioTableModel : public QAbstractTableModel
{
    Q_OBJECT

    QList<Entry*>                   m_entries;
    QMap<QString, Entry*>           m_data_by_key;
    QMultiMap<QString, Entry*>      m_data_by_key_multiple;
    QMap<QString, Entry*>           m_data_by_fullfilename;
    QMultiMap<QString, Entry*>      m_data_by_fullfilename_multiple;
    QMap<QString, Entry*>           m_data_by_filenamestem;
    QMultiMap<QString, Entry*>      m_data_by_filenamestem_multiple;
    QMap<int,Entry*>                m_data_by_size;
    QMultiMap<int,Entry*>           m_data_by_size_multiple;

    bool                            m_isDirty;
public:
    BiblioTableModel(QObject *      parent = nullptr): QAbstractTableModel(parent){};
    BiblioTableModel(QList<Entry*>  entries, QObject *parent = 0);
    MySortFilterProxyModel *        m_proxyModel;

    int                             rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int                             columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant                        data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant                        headerData(int section, Qt::Orientation  value, int role) const override;
    bool                            insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override ;
    bool                            removeRows(int position, int rows, const QModelIndex &index) override;
    bool                            setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    Qt::ItemFlags                   flags(const QModelIndex &index) const override;

    void             add_entry(Entry* entry);
    bool             contains (Entry* entry) {return m_entries.contains(entry);}
    void             replace_entry(int row, Entry*);

    QList< Entry* >  get_entries() {return m_entries;}
    QList<QString>   get_keys() {return m_data_by_key.keys(); }
    Entry*           get_entry_by_key(QString key) {return m_data_by_key[key]; }
    QList<Entry*>    get_multiple_entries_by_key(QString key) {return m_data_by_key_multiple.values(key);}

    void            register_all_entries();// assumes the entries have been entered into m_entries;
    void            register_biblioentry_by_key(Entry * entry );
    void            register_biblioentry_by_size(Entry * entry );
    void            register_biblioentry_by_fullfilename(Entry* entry);
    void            register_biblioentry_by_filenamestem(Entry* entry);

    QList<Entry*>   contains_filenamestem(QString);
    Entry*          contains_fullfilename(QString);
    bool            contains_key(QString key) { return m_data_by_key.contains(key);}

    int             get_count_of_multiply_used_keys(){ return m_data_by_key_multiple.count();}
    QList<QString>  get_keys_used_multiply() {return m_data_by_key_multiple.keys();}
    QList<Entry*>   get_multiple_entries_from_one_key(QString key) { return m_data_by_key_multiple.values(key); }

    int             get_count_of_multiply_used_filenames(){ return m_data_by_filenamestem_multiple.count();}
    QList<QString>  get_filenames_used_multiply() {return m_data_by_filenamestem_multiple.keys();}
    QList<Entry*>   get_multiple_entries_from_one_filename(QString filename) { return m_data_by_filenamestem_multiple.values(filename); }

    int             get_count_of_entries_with_size() { return m_data_by_size.count(); }
    Entry*          get_entry_by_size(int size) ;

    void            register_entry(Entry*);
    void            update_data_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry_onboard);

    bool            get_dirty() {return m_isDirty;}
    void            make_dirty() {m_isDirty = true;}
    void            make_clean() {m_isDirty = false;}
};


#endif // BIBLIOTABLEMODEL_H
