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
#include <QSplitter>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include "tok.h"

struct bibentry;
struct Prefix_String{
    QString m_prefix;
    QString m_string;
    Prefix_String(QString prefix, QString string){
        m_prefix = prefix;
        m_string = string;
    }
};

class List;
class Entry;
class PopUp;
class EntryModel;
class EntryView;
class EntriesModel;
class EntriesView; // tableView
class EntriesView_onboard_files;
class EntryMatchModel;
class EntryMatchView;
class BiblioTableModel;
enum view_function {onboardEntries, biblioEntries, sameSizeEntries};

QString get_first_author(QString names);
QString find_surname(QString name);
QString prepose_surname_if_necessary(QString name);
QString invert_first_author_if_necessary(QString authors);

class MySortFilterProxyModel : public QSortFilterProxyModel{
    Q_OBJECT
};


/*
class TableView : public QTableView{
    Q_OBJECT
public:
    TableView(QObject * parent = 0);
    //void currentChanged(const QM  odelIndex &current, const QModelIndex &previous) override;
};
*/
class NewStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    NewStandardItemModel(QObject * parent);
};
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
class Widget : public QWidget
{
    Q_OBJECT
    friend class EntryModel;
    friend class EntriesModel;
    friend class PopUp;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void bibitem_info_on_middle_table_widget (const QModelIndex & index);

private:
    bool                    m_isDirty; // true means that BiblioTableModel has changed since last saving of it.

    /* Models and views   */

    //MySortFilterProxyModel* proxyModel_for_topTableView;
    BiblioTableModel        * m_biblioModel;
    EntriesModel            * onboard_pdf_model;

    EntriesModel            * m_same_size_files_model;
    EntryMatchModel         * m_entry_match_model;
    QFileSystemModel        * m_file_system_model;
    ToK_model               * m_ToK_model;
    EntryModel              * m_selected_entry_model;

    /*      Entry       */
    Entry                   * m_selected_biblio_entry;
    Entry                   * m_selected_onboard_entry;
    TreeItem                * m_selected_ToK_item;
    Entry                   * m_entry_in_top_table;  // change this to curent selection in model;
    Entry                   * m_entry_in_middle_table; // ditto

    int                      m_number_of_sceen_states;
    int                      m_screen_state;

    // GUI

    QTableView                * m_topTableView;
    QTableView                * m_topTableView2;
    EntryMatchView            * m_entry_match_view;
    //EntryView               * m_center_entry_view;
    QTableView                * m_center_entry_view;
    EntriesView_onboard_files * m_bottomTableView;
    QVBoxLayout               * m_layout;
    QSplitter                 * m_mainSplitter;
    QSplitter                 * m_leftSplitter;
    QSplitter                 * m_rightSplitter;
    QGridLayout               * m_small_grid_layout;
    QTreeView                 * m_directoryView;
    QTreeView                 * m_ToK_view;

    QTableWidget            * m_middle_table_wdget;
    QTableWidget            * m_bottom_table_widget;  // get rid of this
    QTableWidget            * m_bottom_table_widget2;
    QTableWidget            * m_filePrefixTableWidget;
    QWidget                 * m_middle_right_widget;
    PopUp                   * myPopUp;
    QListWidget             * m_listWidget;
    QListWidget             * m_listNamesWidget;
    QLabel                  * m_proposed_new_title_label;

    QLineEdit   * m_proposed_new_title_widget;
    QPushButton * m_create_new_list_button;
    QPushButton * m_create_new_bibentry_button;
    QPushButton * m_generate_new_filename_button;
    QPushButton * m_delete_size_on_selected_biblio_entries;
    QPushButton * m_change_root_directory;
    QPushButton * m_save_biblio_file_button;
    QPushButton * m_add_to_list_button;
    QPushButton * m_link_two_entries;
    QPushButton * m_delete_selected_files_button;
    QPushButton * m_check_biblio_for_shared_key_button;
    QPushButton * m_check_biblio_for_shared_size_button;
    QPushButton * m_check_biblio_for_shared_filename_button;
    QLineEdit * m_new_list_name_widget;

    // Keeping track of GUI
    QStringList     m_bibliography_labels;
    QStringList     m_bibliography_short_labels;
    QString         m_root_folder;
    QString         m_json_folder;
    QString         m_init_folder;
    QString         m_directory_view_root;
    QSettings       m_settings;
    QString         m_prefered_location; // if a copy is found in this folder, then it automatically becomes the primary location of the file.
    int             m_selected_row_in_bottom_table;
    int             m_selected_row_in_bottom_view;

    QMultiMap<QString, Entry*> m_data_by_filenamestem;
    QMultiMap<int,Entry*>      m_data_by_size;
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
    QShortcut *  m_keyCtrlC; // change filename
    QShortcut *  m_keyCtrlF;
    QShortcut *  m_keyCtrlH; // read old fileformat
    QShortcut *  m_keyCtrlJ; // change location of lists
    QShortcut *  m_keyCtrlK;
    QShortcut *  m_keyCtrlL;
    QShortcut *  m_keyCtrlM;  // match files in biblio and onboard
    QShortcut *  m_keyCtrlN;
    QShortcut *  m_keyCtrlO;
    QShortcut *  m_keyCtrlP; // PopUp
    QShortcut *  m_keyCtrlQ; // Quit
    QShortcut *  m_keyCtrlR;
    QShortcut *  m_keyCtrlS;
    QShortcut *  m_keyCtrlU;
    QShortcut *  m_keyCtrlZ; // link top and bottom entries;
    QShortcut *  m_keyCtrlSlash;
    QShortcut *  m_keyCtrlLeftBracket; // [ open popUp
    QShortcut *  m_keyCtrlRightBracket; // ] close popUp
    QShortcut *  m_keyCtrlComma;
    QShortcut *  m_keyCtrlPeriod;
    QShortcut *  m_keyCtrl1; // 1

    bool biblio_model_contains(Entry* entry);
    void list_functionality();

    //void display_entry_on_middle_table();  // this will be deleted
    void display_entry_on_entry_model(EntryModel*, Entry*);
    QList<Entry*>  get_entries_by_size(int);              // this should be removed, changed to biblio model

    void update_data_by_fullfilename (QString, QString, Entry*);
    void update_files_onboard_by_fullfilename(QString, QString, Entry*);
    void update_files_onboard_by_filenamestem(QString, QString, Entry*);
    void update_selected_biblio_entry();
    void load_file_prefixes(QTableWidget*);

    //void put_bibitem_info_on_middle_table_widget(const Entry* entry);
    void establish_screen_layout();
    Entry* get_selected_biblio_entry() {return m_selected_biblio_entry;}
    Entry* get_selected_onboard_entry() {return m_selected_onboard_entry;}

private slots:

    void Control_S();
    void create_new_biblio_entry();
    void create_or_update_biblio_entry();
    void set_screen_layout();
    void set_screen_layout_old();
    void quit_now() {QApplication::quit();}
    void read_JSON_file_old();
    void read_JSON_file_new(QString filename = QString());
    void read_init_file();
    void add_entry_to_top_view(Entry*);

    /* names */
    QString create_new_filename_stem(QString old_name);
    void change_full_filename(QString old_name, QString new_fullname);
    void change_selected_filename();
    void change_onboard_filename(QString new_name);
    void generate_new_filename();
    void add_prefix_to_selected_onboard_filename();


    void create_new_bibentry();
    void delete_selected_files();
    void delete_size_on_selected_biblio_entries();
    QStringList get_bibliography_labels() {return m_bibliography_labels;}
    //TreeItem* get_selected_ToK_line();
    void link_top_and_bottom_entries_from_size();
    void link_top_and_bottom_entries();
    void match_filestems() ;
    void on_top_table_view_clicked(const QModelIndex&);
    void on_top_table_view_doubleClicked(const QModelIndex &);
    void on_middle_table_widget_doubleClicked(int,int);
    void on_bottom_table_view_clicked(const QModelIndex&);
    void on_bottom_table_view_doubleClicked(QModelIndex);
    void on_middle_widget_item_changed(int row, int column);
    void on_ToK_view_selection_changed();
    void open_popUp();
    void hide_popUp();
    void close_popUp();
    void place_entries_with_shared_keys_on_table();
    void place_entries_with_shared_filename_on_table();
    void place_entries_with_shared_size_on_table();
    void promote_file_from_preferred_location(Entry*);
    //void put_bibitem_info_on_middle_table_widget(const QModelIndex & index);
    void put_file_info_on_entry_view(QModelIndex & current_model_index);
    void read_ToK_from_json(QString filename);
    void search_folders_for_pdf();
    void set_new_root_folder();
    void set_screen_state();
    void set_filename_item_bottom_widget(int row, QString new_name);
    void show_files_with_same_size();
    void toggle_screens();
    void toggle_screens_backwards();
    void write_bibliography();
    void write_bibliography_to_json();
    void write_bibliography_to_bibtex();
    void write_bibentry_to_json(Entry *, QJsonObject & );
    void write_ToK_to_file();
    //void keyPressEvent(QKeyEvent * event);
    //void mousePressEvent(QMouseEvent * event);
    //void put_file_info_on_middle_table_widget(int bottom_widget_row);
    //void put_file_info_on_middle_table_widget(Entry*);
    //void put_file_info_on_center_entry_view(Entry*);
    //void put_file_info_on_popup_widget(Entry*);
    //void register_biblioentry_by_key_name_and_size(Entry *);
    //void register_biblioentry_by_fullfilename(Entry*);
    //void register_biblioentry_by_key(Entry*);
    //void register_biblioentry_by_size(Entry*);
    //void register_biblioentry_by_filenamestem(Entry*);
    //QString test_key_for_uniqueness(QString key);
    //void set_new_filename();
    //void on_bottom_table_widget_clicked(const QModelIndex&);
    //void on_bottom_table_widget_doubleClicked(int row, int column);
    //void mark_file_as_indexed(QString);
    //void toggle_right_side();
    //void read_JSON_file();
    //void link_top_and_bottom_entries_from_filename();

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
    //friend class EntryModel;

    QMap<QString,QString>   info;
    //QString               key;
    int                     size;
    bool                    m_selected_for_deletion;
    //QDateTime m_creation_time;`
    QStringList             m_multiple_filenamefulls;
    QStandardItem *         m_top_view_size_item;
    QStandardItem *         m_top_view_filename_item;
    QTableWidgetItem *      m_bottom_view_size_item;
    QTableWidgetItem *      m_bottom_view_filename_item;
    QList<Entry*>           m_links_to_on_board_entries;
    QList<Entry*>           m_links_to_bib_entries;
    QColor                  m_temporary_color;
public:
    Entry();
    Entry(const Entry&);
    Entry(QString this_key) {info.insert("key", this_key); }
    Entry(QString filename, QString path, qint64 size);
    Entry(QString filename, QString path, int size) ;
    ~Entry();

    QString display();

    void append_keywords(QString k){info["keywords"] = get_keywords() + " " + k;}
    void add_to_bib_entries(Entry* entry){m_links_to_bib_entries.append(entry);}
    void add_to_onboard_entries(Entry* entry);
    void add_keywords(QTableWidget *);
    void color_bottom_view_item_for_size();
    void color_bottom_view_item_for_filename();
    void color_top_view_item_for_size();
    void color_top_view_item_for_filename();
    bool contains_info(QString key){return info.contains(key);}
    QString get_info(QString);
    int     get_size()         {return size;}
    QString get_key()          {if (info.contains("key")) {return info["key"];} else{return QString();}}
    QString get_title()        {if (info.contains("title")) {return info["title"];} else{return QString();}}
    QString get_author()       {if (info.contains("author")) {return info["author"];} else{return QString();}}
    QString get_year()         {if (info.contains("year")) {return info["year"];} else{return QString();}}
    QString get_folder()       {if (info.contains("folder")) {return info["folder"];} else{return QString();}}
    QString get_filenamefull();
    QString get_keywords()     {if (info.contains("keywords")) {return info["keywords"];} else{ return QString();}}
    QStringList* get_multiple_filenamefulls() {return &m_multiple_filenamefulls;}
    QString get_filenamestem() {if (info.contains("filenamestem")) {return info["filenamestem"];} else{return QString();}}
    QColor get_temporary_color() {return m_temporary_color;}
    void write_bibentry_to_bibtex( QTextStream & ,QStringList &);
    QList<Entry*> get_on_board_entries(){return m_links_to_on_board_entries;}
    QList<Entry*> get_links_to_bib_entries() {return m_links_to_bib_entries;}
    void mark_bottom_view_entry_as_matched_to_biblio();
    void remove_bottom_view_links();
    void read_json(QJsonObject &);
    void set_key(QString s) {info["key"] = s;}
    void    set_keywords(QString s) {info["keywords"] = s;}
    void set_title(QString s)  {info["title"] = s;}
    void set_author(QString s) {info["author"] = s;}
    void set_year(QString s) {info["year"] = s;}
    void set_filenameFull(QString s);// {info["filenamefull"] = s;}
    void set_filenameStem(QString s) {info["filenamestem"] = s;}
    void set_folder(QString folder) {info["folder"] = folder;}
    void set_size(int s) {size = s;}
    bool selected_for_deletion() {return m_selected_for_deletion;}
    void set_selected_for_deletion(bool value) {m_selected_for_deletion = value;}
    void set_top_view_size_item (QStandardItem* item ){m_top_view_size_item = item;}
    void set_bottom_view_size_item(QTableWidgetItem* item ) {m_bottom_view_size_item = item;}
    void set_top_view_filename_item (QStandardItem* item ){m_top_view_filename_item = item;}
    void set_bottom_view_filename_item(QTableWidgetItem* item ) {m_bottom_view_filename_item = item;}
    void set_info(QString key,QString value); //{info[key] = value;}
    void set_info(QString key, int this_size) {size = this_size; }
    void set_size_item(int);
    void set_temporary_color(QColor color) {m_temporary_color = color;}
    void set_filename_item_bottom(QString);
    // keys: key, title, author, year, filenameFull, filenameStem



};

class List {
    QString name;
      QList<Entry*> entries;

public:
    List();
    List(QString);
    ~List();
    QString         get_name() {return name;}
    QList<Entry*> * get_entries() {return & entries;}
    Entry*          get_entry(int n) {return entries[n];}
    int             get_count_of_entries(){return entries.count();}
    void            add_entry(Entry * entry) {entries.append(entry);}
    void            read_json(QJsonObject & );
    void            read_json(QJsonArray & );

};
//----------------------------------------------------------------------------------------
class PopUp: public   QTableView
{
    Q_OBJECT

    Entry      * m_entry;
    EntryModel * m_entryModel;
    Widget     * m_main_widget;
public:
    //PopUp(Entry* , Widget* main_widget);
    PopUp(EntryModel*);
    void setEntry(Entry*);
    void hide();
    void close();
    Widget* get_main_widget() {return m_main_widget;}

};
//----------------------------------------------------------------------------------------

class EntriesView: public QTableView{
    Q_OBJECT
public:
    void set_proxy_model(MySortFilterProxyModel *);
    //void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected); // from AbstractItemView

};




class EntriesView_onboard_files: public QTableView{
    Q_OBJECT
public:
    EntriesView_onboard_files();
    void set_proxy_model(MySortFilterProxyModel *);

};



//.......................................................................................
class EntryMatchModel: public QAbstractTableModel{
    Q_OBJECT

    BiblioTableModel *  m_biblio_model;
    EntriesModel *      m_file_model;
    QList<Entry*>       m_entries;
public:
    EntryMatchModel(BiblioTableModel * biblio_model, EntriesModel *, QWidget*);

    void     match_filestems();
    int      rowCount(const QModelIndex &index ) const;
    int      columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex & index, int role )const;
};

class EntryMatchView: public QTableView{
    Q_OBJECT

public:
    EntryMatchView();
};
//............................................................................................


class EntriesModel: public QAbstractTableModel{
    Q_OBJECT
    Widget *                   m_parent;
    //Entry*                   m_entry;
    Entry*                     m_selected_entry; // ?? is this needed?
    QList<Entry*>              m_entries;
    QMap<QString, Entry*>      m_map_by_filenamestem;
    QMultiMap<QString, Entry*> m_multimap_by_filenamestem;
    QMap<QString, Entry*>       m_map_by_filenamefull;
public:
    EntriesModel ( Widget *parent );
    ~EntriesModel();
    MySortFilterProxyModel *   m_proxyModel;

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
    MySortFilterProxyModel* getProxyModel() {return m_proxyModel;}
    int            number_of_entries(){return m_entries.count();}
    QList<Entry*> * get_entries(){return & m_entries;}
    Entry*          get_entry_with_filenamestem(QString stem) {return m_map_by_filenamestem.value(stem);}
    QList<Entry*>   get_entries_with_filenamestem(QString stem) {return m_multimap_by_filenamestem.values(stem);}
    bool            contains_entry_with_filenamestem(QString stem) {return m_map_by_filenamestem.contains(stem);}
    bool            contains_multiple_entries_with_filenamestem(QString stem){return m_multimap_by_filenamestem.contains(stem);}
    bool            contains(Entry * entry) { return m_entries.contains(entry);}
};
//....................................................................................................
class EntryModel : public QAbstractListModel{
    //friend class Widget;

    Q_OBJECT
    Entry*          m_entry;
    QStringList    m_bibliography_labels;

public:
                   EntryModel (Entry *, QStringList bibliography_labels);
                   ~EntryModel();
    int            rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QStringList    get_bibliography_labels() const  { return   m_bibliography_labels;}
    int             get_filename_row();
    bool           setData(const QModelIndex &index, const QVariant & value, int role) override;
    Qt::ItemFlags  flags(const QModelIndex &index) const override;
    void           display(Entry* entry);
    void           change_entry(Entry*);
//    void           dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
//    void           setData(Entry* entry);
//    bool           setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};


class EntryView: public QTableView{
    Q_OBJECT
public:
    EntryView(){};
};



//.................................................................................................................

#endif // WIDGET_H

