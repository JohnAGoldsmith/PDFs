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

class FileModel;

struct bibentry;
struct Prefix_String{
    QString m_prefix;
    QString m_string;
    Prefix_String(QString prefix, QString string){
        m_prefix = prefix;
        m_string = string;
    }
};

//class List;
class Entry;
class PopUp;
class EntryModel;
class EntryView;
class FilesModel;
class EntriesView; // tableView
class TableView_for_files;
class EntryMatchModel;
class EntryMatchView;
class BiblioTableModel;
class File;
enum view_function {onboardEntries, biblioEntries, sameSizeEntries};

QString get_first_author(QString names);
QString find_surname(QString name);
QString prepose_surname_if_necessary(QString name);
QString invert_first_author_if_necessary(QString authors);

class My_filename_SortFilterProxyModel : public QSortFilterProxyModel{
    Q_OBJECT
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};


class My_biblio_SortFilterProxyModel : public QSortFilterProxyModel{
    Q_OBJECT
};



// this is not used, should be tossed:
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
    friend class FilesModel;
    friend class FileModel;
    friend class PopUp;


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void bibitem_info_on_middle_table_widget (const QModelIndex & index);

private:
    bool                    m_isDirty; // true means that BiblioTableModel has changed since last saving of it.

    /* Models and views   */

    BiblioTableModel        * m_biblio_model;
    FilesModel              * m_onboard_pdf_model;

    FilesModel              * m_same_size_files_model;
    EntryMatchModel         * m_entry_match_model;
    QFileSystemModel        * m_file_system_model;
    ToK_model               * m_ToK_model;

    EntryModel              * m_selected_entry_model;
    FileModel               * m_selected_file_model;

    /*      Entry       */
    Entry                   * m_selected_biblio_entry;
    File                    * m_selected_file;
    TreeItem                * m_selected_ToK_item;

    int                      m_number_of_sceen_states;
    int                      m_screen_state;

    // GUI

    QTableView                * m_top_tableView_biblio;
    //QTableView                * m_top_tableView2;
    QTableView                * m_center_tableView1_file; // for selected file
    QTableView                * m_center_tableView2_entry;  // for selected (biblio) entry
    QTableView                * m_bottom_tableView;
    QTableView                * m_entry_match_view;
    QWidget                   * m_middle_right_widget;
    QTableWidget              * m_bottom_table_widget;
    QTableWidget              * m_bottom_table_widget2;


    QVBoxLayout               * m_layout;
    QSplitter                 * m_mainSplitter;
    QSplitter                 * m_leftSplitter;
    QSplitter                 * m_rightSplitter;
    QGridLayout               * m_small_grid_layout;
    QTreeView                 * m_directoryView;
    QTreeView                 * m_ToK_view;
    PopUp                     * myPopUp;
    QLabel                    * m_proposed_new_title_label;

    QLineEdit   * m_proposed_new_title_widget;
    QPushButton * m_create_new_list_button;
    QPushButton * m_create_new_bibentry_button;
    QPushButton * m_generate_new_filename_button;
    QPushButton * m_change_file_name_button;
    QPushButton * m_delete_size_on_selected_biblio_entries;
    QPushButton * m_change_root_directory;
    QPushButton * m_save_biblio_file_button;
    QPushButton * m_add_to_list_button;
    QPushButton * m_link_two_entries;
    QPushButton * m_delete_selected_files_button;
    QPushButton * m_check_biblio_for_shared_key_button;
    QPushButton * m_check_biblio_for_shared_size_button;
    QPushButton * m_check_biblio_for_shared_filename_button;
    QLineEdit   * m_new_list_name_widget;
    QPushButton * m_new_ToK_item_button;
    QLineEdit   * m_new_ToK_prefix_widget;
    QLineEdit   * m_new_ToK_prose_widget;

    // Keeping track of GUI
    QStringList     m_bibliography_labels;
    QStringList     m_bibliography_short_labels;
    QString         m_root_folder;
    QString         m_json_folder;
    QString         m_init_folder;
    QString         m_directory_view_root;
    QSettings       m_settings;
    QString         m_prefered_location; // if a copy is found in this folder, then it automatically becomes the primary location of the file.
    //int             m_selected_row_in_bottom_table;
    //int             m_selected_row_in_bottom_view;

    QMultiMap<QString, Entry*> m_data_by_filenamestem;
    QMultiMap<int,Entry*>      m_data_by_size;
    QMap<QString, Entry*>      m_files_onboard_by_filenamefull;
    QMultiMap<QString, File*> m_files_onboard_by_filenamestem;
    QMultiMap<int, File*>     m_files_onboard_by_size;
    QMultiMap<QString, Entry*> m_data_key_collisions;
    QMultiMap<QString, Entry*> m_filename_collisions;



    QShortcut *  m_keyCtrlA; // show files of same size
    QShortcut *  m_keyCtrlC; // change filename
    QShortcut *  m_keyCtrlD; // create new filename
    QShortcut *  m_keyCtrlE; // change selected file name to one indicated
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


    Entry* put_selected_file_into_new_biblio_entry(File*, Entry*);

    //bool biblio_model_contains(Entry* entry);
    void list_functionality();

    void    display_entry_on_entry_model(EntryModel*, Entry*);
    QList<Entry*>  get_entries_by_size(int);              // this should be removed, changed to biblio model
    QStringList get_bibliography_labels() {return m_bibliography_labels;}

    // should these exist?
    void    update_data_by_fullfilename (QString, QString, Entry*);
    void    update_files_onboard_by_fullfilename(QString, QString, Entry*);
    void    update_files_onboard_by_filename(QString, QString, Entry*);
    void    update_selected_biblio_entry();
    void    load_file_prefixes(QTableWidget*);

    void    establish_screen_layout();
    Entry*  get_selected_biblio_entry() {return m_selected_biblio_entry;}
    File*   get_selected_onboard_entry() {return m_selected_file;}
    void    link_biblio_entry_and_file(Entry* biblio, File* onboard);



private slots:
    void search_folders_for_pdf();

    void add_ToK_item();
    void Control_S();
    void create_new_biblio_entry();
    void create_or_update_biblio_entry();
    void set_screen_layout();
    void quit_now() {QApplication::quit();}
    void read_JSON_file_old();
    void read_JSON_file_new(QString filename = QString());
    void read_init_file();

    /* names */
    void change_full_filename(QString old_name, QString new_fullname);
    //void change_selected_filename();
    void change_selected_file_filename();
    void generate_new_filename_from_selected_biblio_entry();
    void add_prefix_to_selected_onboard_filename();

    void delete_selected_files();
    void delete_size_on_selected_biblio_entries();

    QString get_selected_ToK_item_with_spaces();
    void link_biblio_entries_and_files_by_size();
    void link_biblio_entries_and_onboard_entries_by_filename();

    void match_filestems() ;
    void on_top_table_view_clicked(const QModelIndex&);
    void on_top_table_view_doubleClicked(const QModelIndex &);
    void on_bottom_table_view_clicked(const QModelIndex&);
    void on_bottom_table_view_doubleClicked(QModelIndex);
    void on_middle_widget_item_changed(int row, int column);
    void on_ToK_view_selection_changed();

    void open_popUp();
    void hide_popUp();
    void close_popUp();

    void place_entries_with_shared_keys_on_table();
    void place_entries_with_shared_filename_on_table();
    void place_biblio_entries_with_shared_size_on_table();


    void put_selected_file_into_new_selected_biblio_entry(File* file, Entry* entry);
    void read_ToK_from_json(QString filename);
    void set_new_root_folder();
    void set_screen_state();
    void set_filename_item_bottom_widget(int row, QString new_name);
    //void show_files_with_same_size();
    void toggle_screens();
    void toggle_screens_backwards();
    void write_bibliography();
    void write_bibliography_to_json();
    void write_bibliography_to_bibtex();
    void write_bibentry_to_json(Entry *, QJsonObject & );
    void write_ToK_to_file();

    //void promote_file_from_preferred_location(Entry*);
    //void put_bibitem_info_on_middle_table_widget(const QModelIndex & index);




};
// ------------------------------------------------------------------------------------------------------ //


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
    void set_proxy_model(My_filename_SortFilterProxyModel *);
    //void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected); // from AbstractItemView

};




class TableView_for_files: public QTableView{
    Q_OBJECT
public:
    TableView_for_files();
    void set_proxy_model(My_filename_SortFilterProxyModel *);

};

//............................................................................................


//....................................................................................................


class EntryView: public QTableView{
    Q_OBJECT
public:
    EntryView(){};
};



//.................................................................................................................

#endif // WIDGET_H

