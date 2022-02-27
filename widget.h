 #ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStringList>
#include <QSettings>
#include <QKeyEvent>
#include <QApplication>
#include <QJsonObject>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QList>
#include <QComboBox>
#include <QMultiMap>
#include <QShortcut>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>


struct bibentry;
class List;
class Entry;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject * parent = nullptr);
    TableModel(QList<Entry*> entries, QObject *parent = 0);

    //int rowCount (const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //int columnCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    //QVariant data(const QModelIndex &index, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation  value, int role) const override;

    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //QList< Entry* > getList();
    void add_entry(Entry* entry){ m_entries.append(entry); }

private:
    QList<Entry*> m_entries;
};

class TableView : public QTableView{
    Q_OBJECT
public:
    TableView(QObject * parent = 0);

};
class NewStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    NewStandardItemModel(QObject * parent = 0);
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void put_bibitem_info_on_middle_table_widget (const QModelIndex & index);

private:

    TableModel         * biblioModel;
    QStandardItemModel * bibModel;


    QTableView * topTableView;
    QSortFilterProxyModel * proxyModel_for_topTableView;

    QTableWidget * middleTableWidget;
    QTableWidget * bottomTableWidget;
    QTableWidget * filePrefixTableWidget;
    QTreeView * directoryView;
    QListWidget * listWidget;
    QListWidget * listNamesWidget;
    QLabel    * m_proposed_new_title_label;
    QLineEdit * m_proposed_new_title_widget;
    QPushButton * m_change_root_directory;
    QPushButton * m_change_filename_button;
    QPushButton * m_create_new_list_button;
    QPushButton * m_save_biblio_file_button;
    QPushButton * m_add_to_list_button;
    QPushButton * m_link_two_entries;
    QPushButton * m_delete_selected_files_button;
    QLineEdit * m_new_list_name_widget;


    QStringList m_bibliography_labels;
    QStringList m_bibliography_short_labels;
    QString m_root_folder;
    QString m_json_folder;
    QSettings m_settings;
    QString m_prefered_location; // if a copy is found in this folder, then it automatically becomes the primary location of the file.
    Entry* m_entry_in_top_table;
    Entry* m_entry_in_middle_table;
    Entry* m_entry_in_bottom_table;
    int m_selected_row_in_top_table;


    QMap<QString, Entry*> m_data_by_key;
    QMap<QString, Entry*> m_data_by_fullfilename;
    QMultiMap<QString, Entry*> m_data_by_filenamestem;
    QMultiMap<int,Entry*> m_data_by_size;
    QMap<QString, Entry*>      m_files_onboard_by_filenamefull;
    QMultiMap<QString, Entry*> m_files_onboard_by_filenamestem;
    QMultiMap<int, Entry*>     m_files_onboard_by_size;
    QMultiMap<QString, Entry*> m_data_key_collisions;
    QMultiMap<QString, Entry*> m_filename_collisions;

    /*   Lists         */
    QString              m_lists_complete_filename;
    List *               m_current_list;
    QList<List*>         m_Lists;
    QMap<QString, List*> m_Lists_map;

    QShortcut *  m_keyCtrlA; // show files of same size
    QShortcut *  m_keyCtrlF;
    QShortcut *  m_keyCtrlH; // read old fileformat
    QShortcut *  m_keyCtrlJ; // change location of lists
    QShortcut *  m_keyCtrlK;
    QShortcut *  m_keyCtrlL;
    QShortcut *  m_keyCtrlN;
    QShortcut *  m_keyCtrlO;
    QShortcut *  m_keyCtrlQ; // Quit
    QShortcut *  m_keyCtrlR;
    QShortcut *  m_keyCtrlS;
    QShortcut *  m_keyCtrlU;
    QShortcut *  m_keyCtrlZ; // link top and bottom entries;

    void display_entry_on_middle_table();
    QList<Entry*>  get_entries_by_size(int);

    void update_data_by_fullfilename (QString, QString, Entry*);
    void update_files_onboard_by_fullfilename(QString, QString, Entry*);
    void update_files_onboard_by_filenamestem(QString, QString, Entry*);
    void load_file_prefixes(QTableWidget*);

private slots:

    void quit_now() {QApplication::quit();}
    void keyPressEvent(QKeyEvent * event);
    void read_JSON_file_old();
    void read_JSON_file_new();
    //void read_JSON_file();
    void search_folders_for_pdf();
    void set_new_root_folder();
    void write_bibliography();
    void write_bibliography_to_json();
    void write_bibliography_to_bibtex();
    void promote_file_from_preferred_location(Entry*);

    void add_entry_to_bibmodel(Entry *);
    void add_entry_to_bibmodel_by_fullfilename(Entry*);
    void add_entry_to_bibmodel_by_key(Entry*);
    void add_entry_to_bibmodel_by_size(Entry*);
    void add_entry_to_bibmodel_by_filenamestem(Entry*);
    void add_entry_to_top_view(Entry*);
    void delete_selected_files();
    void write_bibentry_to_json(Entry *, QJsonObject & );
    void generate_new_title();
    void put_file_info_on_middle_table_widget(int bottom_widget_row);
    void on_top_table_view_clicked(const QModelIndex &  );
    void on_top_table_view_doubleClicked(const QModelIndex &);
    void on_bottom_table_widget_clicked(const QModelIndex & );
    void on_bottom_table_widget_doubleClicked(int row, int column);
    void on_middle_widget_item_changed(int row, int column);
    void set_new_filename();
    void mark_file_as_indexed(QString);
    void link_top_and_bottom_entries_from_size();
    void link_top_and_bottom_entries_from_filename();
    void link_top_and_bottom_entries();
    QString test_key_for_uniqueness(QString key);
    void show_files_with_same_size();

    /*    Lists              */
    //void read_lists_file();
    void new_list();
    void new_list(QString);
    void add_entry_to_list();
    void add_entry_to_list(List*, Entry*);
    //void write_lists_to_json(QJsonObject &);
    void write_lists_to_json(QJsonArray &);
    void change_location_of_listsInfo();

    void display_list(List* );
    void select_new_list(QListWidgetItem * );
    void on_listWidget_doubleClicked(QListWidgetItem*);
    void save_current_list();

};
// ------------------------------------------------------------------------------------------------------ //

class Entry {
    QMap<QString,QString> info;
    QString key;
    int size;
    QDateTime m_creation_time;
    QStringList m_multiple_filenamefulls;
    QStandardItem * m_top_view_size_item;
    QStandardItem * m_top_view_filename_item;
    QTableWidgetItem * m_bottom_view_size_item;
    QTableWidgetItem * m_bottom_view_filename_item;
    QList<Entry*> m_links_to_on_board_entries;
    QList<Entry*> m_links_to_bib_entries;
public:
    Entry();
    Entry(QString this_key) {key = this_key;}
    Entry(QString filename, QString path, qint64 size);
    Entry(QString filename, QString path, int size);
    ~Entry();
    int     get_size() {return size;}
    QString get_key() {if (info.contains("key")) {return info["key"];} else{return QString();}}
    QString get_title() {if (info.contains("title")) {return info["title"];} else{return QString();}}
    QString get_author() {if (info.contains("author")) {return info["author"];} else{return QString();}}
    QString get_year() {if (info.contains("year")) {return info["year"];} else{return QString();}}
    QString get_folder()  {if (info.contains("folder")) {return info["folder"];} else{return QString();}}
    QString get_filenamefull() {if (info.contains("filenamefull")) {return info["filenamefull"];} else{return QString();}}
    QString get_keywords() {if (info.contains("keywords")) {return info["keywords"];} else{ return QString();}}
    QStringList* get_multiple_filenamefulls() {return &m_multiple_filenamefulls;}
    QString get_filenamestem() {if (info.contains("filenamestem")) {return info["filenamestem"];} else{return QString();}}
    void write_bibentry_to_bibtex( QTextStream & ,QStringList &);

    //QTableWidgetItem * get_bottom_view_filename_item() {return m_bottom_view_filename_item;}
    QList<Entry*> get_on_board_entries(){return m_links_to_on_board_entries;}
    QList<Entry*> get_links_to_bib_entries() {return m_links_to_bib_entries;}
    void color_bottom_view_item_for_size();
    void color_bottom_view_item_for_filename();
    void color_top_view_item_for_size();
    void color_top_view_item_for_filename();
    void set_key(QString s) {info["key"] = s;}
    void set_title(QString s)  {info["title"] = s;}
    void set_author(QString s) {info["author"] = s;}
    void set_year(QString s) {info["year"] = s;}
    void set_filenameFull(QString s);// {info["filenamefull"] = s;}
    void set_filenameStem(QString s) {info["filenamestem"] = s;}
    void set_folder(QString folder) {info["folder"] = folder;}
    void set_size(int s) {size = s;}
    void append_keywords(QString k){info["keywords"] = get_keywords() + " " + k;}
    void add_to_bib_entries(Entry* entry){m_links_to_bib_entries.append(entry);}
    void add_to_on_board_entries(Entry* entry) {m_links_to_on_board_entries.append(entry);}
    void set_top_view_size_item (QStandardItem* item ){m_top_view_size_item = item;}
    void set_bottom_view_size_item(QTableWidgetItem* item ) {m_bottom_view_size_item = item;}
    void set_top_view_filename_item (QStandardItem* item ){m_top_view_filename_item = item;}
    void set_bottom_view_filename_item(QTableWidgetItem* item ) {m_bottom_view_filename_item = item;}
    void remove_bottom_view_links();
    void read_json(QJsonObject &);
    void set_info(QString key,QString value); //{info[key] = value;}
    void set_info(QString key, int this_size) {size = this_size;}
    void set_size_item(int);
    void set_filename_item_bottom(QString);
    bool contains_info(QString key){return info.contains(key);}   
    QString get_info(QString);
    // keys: key, title, author, year, filenameFull, filenameStem
    QString display();
    //QStandardItem    * get_top_view_filename_item() {return m_top_view_filename_item;}
    //QTableWidgetItem * get_bottom_view_size_item() {return m_bottom_view_size_item;}
    //QStandardItem    * get_top_view_size_item() {return m_top_view_size_item;}


};

class List {
    QString name;
      QList<Entry*> entries;

public:
    List();
    List(QString);
    ~List();
    QString get_name() {return name;}
    QList<Entry*> * get_entries() {return & entries;}
    Entry* get_entry(int n) {return entries[n];}
    int get_count_of_entries(){return entries.count();}
    void add_entry(Entry * entry) {entries.append(entry);}
    void read_json(QJsonObject & );
    void read_json(QJsonArray & );

};

#endif // WIDGET_H
