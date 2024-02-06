#include <QGridLayout>
#include <QDir>
#include <QFileInfo>
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
#include "BiblioTableModel.h"
#include "widget.h"
#include "FilesModel.h"
#include "file_model.h"
#include "EntryModel.h"
#include "Entry.h"
#include "file.h"
#include "entry_match_model.h"

//class List;

Qt::ItemFlags BiblioTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void Widget::set_screen_layout(){
    myPopUp = nullptr;

    /*     Layout and main Splitter     */
    if (m_layout) delete m_layout;
    m_layout = new QVBoxLayout(this);
    if (m_mainSplitter) delete m_mainSplitter;
    m_mainSplitter = new QSplitter (Qt::Horizontal,this);

    m_mainSplitter->sizePolicy();
    m_mainSplitter->setStretchFactor(0,10);
    //m_mainSplitter->setStretchFactor(1,10);
    m_layout->addWidget(m_mainSplitter);

    // top level
    m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
    m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);
    /*  Left, Middle , Right of screen       */
    m_mainSplitter->addWidget(m_leftSplitter);
    m_mainSplitter->addWidget(m_center_tableView1_file);
    m_mainSplitter->addWidget(m_center_tableView2_entry);
    m_mainSplitter->addWidget(m_rightSplitter);
    m_mainSplitter->setSizes(QList<int>() << 1500 << 800 << 800 << 1000);

    // left column
    m_leftSplitter->addWidget(m_top_tableView_biblio);
    m_leftSplitter->addWidget(m_bottom_table_widget); // used for displaying shared stem-names, sizes, etc. Reports to user...
    m_leftSplitter->addWidget(m_bottom_table_widget2); // used for displaying shared stem-names, sizes, etc. Reports to user...
    m_leftSplitter->addWidget(m_bottom_tableView);
    m_leftSplitter->addWidget(m_entry_match_view); // not currently used
    //m_entry_match_view->setVisible(false);

    // right column
    // 1. grid layout first:
    m_small_grid_layout   = new QGridLayout;
    m_middle_right_widget = new QWidget(this);
    m_middle_right_widget->setLayout(m_small_grid_layout);
    m_proposed_new_title_label = new QLabel("Proposed new title") ;

    m_proposed_new_title_widget = new QLineEdit();

    m_new_ToK_item_button = new QPushButton();
    m_new_ToK_item_button->setText("New ToK item");
    m_new_ToK_prefix_widget = new QLineEdit();
    //m_new_ToK_prefix_widget->setFixedWidth(120);
    m_new_ToK_prose_widget = new QLineEdit();
    //m_new_ToK_prose_widget->setFixedWidth(200);
    m_change_file_name_button = new QPushButton ("Change filename (^E)");
    m_generate_new_filename_button = new QPushButton("Generate new filename and key (^D)");
   //m_change_filename_button = new QPushButton("Change file name to (^K):");
    m_create_new_bibentry_button = new QPushButton("Create new biblio entry (^K):");
    m_change_root_directory = new QPushButton("Change root directory");
    m_save_biblio_file_button = new QPushButton("Save biblio file (^S)");
    m_link_two_entries = new QPushButton("Link top and bottom entries (^Z)");
    m_delete_selected_files_button = new QPushButton("Delete selected files");
    m_delete_size_on_selected_biblio_entries = new QPushButton("Delete size on selected biblio entries");
    m_check_biblio_for_shared_key_button = new QPushButton("Check biblio for shared keys");
    m_check_biblio_for_shared_size_button = new QPushButton("Check biblio for shared sizes");
    m_check_biblio_for_shared_filename_button = new QPushButton("Check biblio for shared filenames");

    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::yellow); // not used?
    sample_palette.setColor(QPalette::WindowText, Qt::green); // not used?
    setStyleSheet("QLineEdit { background-color: yellow }");
    setStyleSheet("Widget{ background-color: green}");
    setStyleSheet("ToK_model { background-color: green}");

    m_small_grid_layout->addWidget(m_generate_new_filename_button,1,0);
    m_small_grid_layout->addWidget(m_proposed_new_title_widget,1,1);
    m_small_grid_layout->addWidget(m_change_file_name_button, 2,0);
    m_small_grid_layout->addWidget(m_create_new_bibentry_button,3,0);
    m_small_grid_layout->addWidget(m_delete_size_on_selected_biblio_entries,4,0);
    m_small_grid_layout->addWidget(m_change_root_directory,5,0);
    m_small_grid_layout->addWidget(m_save_biblio_file_button,6,0);
    //m_small_grid_layout->addWidget(m_add_to_list_button,6,0);
    m_small_grid_layout->addWidget(m_link_two_entries,7,0);
    m_small_grid_layout->addWidget(m_delete_selected_files_button,8,0);
    m_small_grid_layout->addWidget(m_check_biblio_for_shared_key_button,9,0);
    m_small_grid_layout->addWidget(m_check_biblio_for_shared_size_button,10,0);
    m_small_grid_layout->addWidget(m_check_biblio_for_shared_filename_button,11,0);
    m_small_grid_layout->addWidget(m_new_ToK_item_button,12,0);
    m_small_grid_layout->addWidget(m_new_ToK_prefix_widget,12, 1);
    m_small_grid_layout->addWidget(m_new_ToK_prose_widget, 13,1);


    // 2.  the right column:
    m_rightSplitter->addWidget(m_ToK_view);
    m_rightSplitter->addWidget(m_middle_right_widget); // this is the grid-layout
    m_rightSplitter->addWidget(m_directoryView);
   //m_rightSplitter->setStretchFactor(0,60);
   // m_rightSplitter->setStretchFactor(2,40);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPalette pal = QPalette();

    // set black background
    // Qt::black / "#000000" / "black"
    pal.setColor(QPalette::Window, Qt::yellow);
    setAutoFillBackground(true);
    setPalette(pal);
    showMaximized();
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    setFocus();
    m_isDirty = false;
    m_bibliography_labels  <<  "filename" <<  "folder"<< "size"<< "author" << "title" << "year" << "key" << "fillnamefull" <<
                                "date_created" << "date_last_read" << "date_last_modified"<<
                               "type"<< "booktitle"<< "month" << "organization" << "series" << "number" << "volume" << "doi" <<
                               "file (not found)" << "issn"<< "isbn" <<  "url" << "pages" << "abstract" <<  "address" <<
                               "annote" << "mendeley-tags" << "publisher" << "journal"  << "editor" << "school" << "type" <<
                               "archiveprefix" << "arxivid" << "pmid" << "eprint" << "possible file" << "pmid" << "edition" <<
                               "primaryclass" << "chapter" << "institution" << "howpublished" << "candidates" << "file" << "translator" <<
                               "date_entry_created";
    m_bibliography_short_labels  << "author" << "title" << "year" << "key" << "filename" << "size" << "folder" << "keywords";
    m_prefered_location = "Dropbox/library/";

    QSettings settings ("JohnAGoldsmith", "PdfManager");
    m_root_folder = m_settings.value("rootfoldername", "/home/john/Dropbox/").toString();
    m_json_folder = m_settings.value("jsonfoldername", "/home/").toString();
    m_init_folder = m_settings.value("initfoldername", "/home/").toString();

    QPalette palette = QPalette();
    palette.setColor(QPalette::Window,QColor(0,0,200,250));
    setPalette(palette);

    m_mainSplitter = nullptr;
    m_selected_file = nullptr;
    m_selected_biblio_entry = nullptr;
    m_selected_ToK_item = nullptr;
    m_bottom_table_widget = new QTableWidget;
    m_bottom_table_widget2 = new QTableWidget;

    // for the selected entry
    m_center_tableView2_entry = new QTableView();
    m_center_tableView2_entry->horizontalHeader()->setStretchLastSection(QHeaderView::Stretch);
    //m_center_tableView2_entry->setMaximumWidth(1400);
    //m_center_tableView2_entry->setMinimumWidth(500);
     m_selected_entry_model  = new EntryModel(new Entry(), get_bibliography_labels());
    m_center_tableView2_entry->setModel(m_selected_entry_model);
     //m_center_tableView2_entry->setColumnWidth(0,160);

    // for the selected file
    m_center_tableView1_file = new QTableView();
    m_center_tableView1_file->horizontalHeader()->setStretchLastSection(QHeaderView::Stretch);
    m_center_tableView1_file->setMaximumWidth(1000);
    //m_center_tableView1_file->setMinimumWidth(500);
    m_selected_file_model  = nullptr;
    //m_center_tableView1_file->setColumnWidth(0,160);

    m_directoryView = new QTreeView;

    // Models
    m_onboard_pdf_model = new FilesModel(this);
    m_same_size_files_model = new FilesModel(this);
    m_biblio_model = nullptr;
    m_selected_file_model = nullptr;
    m_ToK_model = new ToK_model(this);
    read_ToK_from_json("pdf_manager_tok_init.json");

    // Views
    // Left side
    m_top_tableView_biblio = new QTableView(this);
    m_top_tableView_biblio->setSelectionMode(QAbstractItemView::SingleSelection);
    m_top_tableView_biblio->setSortingEnabled(true);
    m_bottom_tableView = new TableView_for_files();
    m_entry_match_view = new QTableView();
    m_file_system_model = new QFileSystemModel(this);
    m_file_system_model->setFilter(QDir::Dirs);
    m_file_system_model->setRootPath("/home/john/");
    m_ToK_view = new QTreeView(this);
    m_ToK_view->setModel(m_ToK_model);
    m_ToK_view->expandAll();
    m_ToK_view->setSelectionMode(QTreeView::SingleSelection);
    m_directoryView->setModel(m_file_system_model);
    m_directoryView->setColumnWidth(0,400);
    m_directoryView->setColumnWidth(1,100);
    m_directoryView->setColumnWidth(2,400);
    m_directoryView->setSortingEnabled(true);

    m_layout = nullptr;
    m_screen_state = 0;
    set_screen_layout();

    m_entry_match_model = new EntryMatchModel(m_biblio_model, m_onboard_pdf_model, this);

    /*      Single clicks                      */
    connect(m_top_tableView_biblio, &QTableView::clicked,
             this , &Widget::on_top_table_view_clicked);
    connect(m_bottom_tableView, &EntriesView::clicked,
            this, &Widget::on_bottom_table_view_clicked );
    connect(m_generate_new_filename_button, &QPushButton::clicked,
            this, &Widget::generate_new_filename_from_selected_biblio_entry);
    connect(    m_change_file_name_button, &QPushButton::clicked,
                this, &Widget::change_selected_file_filename);
    connect(m_delete_size_on_selected_biblio_entries,SIGNAL(clicked()) ,
            this , SLOT(delete_size_on_selected_biblio_entries())  );
    connect(m_create_new_bibentry_button,SIGNAL(clicked()),
            this,SLOT(create_new_biblio_entry()));
          this,SLOT(select_new_list(QListWidgetItem*));
    connect(m_save_biblio_file_button,SIGNAL(clicked()),
                this,SLOT(write_bibliography()));
    connect(m_link_two_entries,SIGNAL(clicked()),                         // to do todo this should be replaced by "link_biblio_entry_and_onboard_entry"
                this,SLOT(link_biblio_entry_and_onboard_entry()));
    connect(m_delete_selected_files_button,SIGNAL(clicked()),
                this,SLOT(delete_selected_files()));
    connect(m_check_biblio_for_shared_key_button,SIGNAL(clicked()),
                this,SLOT(place_entries_with_shared_keys_on_table()));
    connect(m_check_biblio_for_shared_filename_button,SIGNAL(clicked()),
                this,SLOT(place_entries_with_shared_filename_on_table()));
    connect(m_check_biblio_for_shared_size_button,SIGNAL(clicked()),
                this,SLOT(place_biblio_entries_with_shared_size_on_table()));
    connect(m_ToK_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Widget::on_ToK_view_selection_changed);
    connect(m_new_ToK_item_button, &QPushButton::clicked,
            this, &Widget::add_ToK_item);


    // Double clicks
    connect(m_top_tableView_biblio, &QTableView::doubleClicked,
             this , &Widget::on_top_table_view_doubleClicked);

    connect(m_bottom_tableView, &QAbstractItemView::doubleClicked,
            this, &Widget::on_bottom_table_view_doubleClicked);

    connect(m_change_root_directory,SIGNAL(clicked()),
                this,SLOT(set_new_root_folder()));

    /* open and close pop-up */
    m_keyCtrlLeftBracket = new QShortcut(this);
    m_keyCtrlLeftBracket->setKey(Qt::CTRL  | Qt::Key_BracketLeft);
    connect(m_keyCtrlLeftBracket, SIGNAL(activated()), this, SLOT(open_popUp()));
    m_keyCtrlRightBracket = new QShortcut(this);
    m_keyCtrlRightBracket->setKey(Qt::CTRL  | Qt::Key_BracketRight);
    connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(close_popUp()));

    /*  show files with same size */
    m_keyCtrlA = new QShortcut(this);
    m_keyCtrlA->setKey(Qt::CTRL | Qt::Key_A);
    connect(m_keyCtrlA, SIGNAL(activated()), this, SLOT(show_files_with_same_size()));

    /*  chreate possible new file name */
    m_keyCtrlD = new QShortcut(this);
    m_keyCtrlD->setKey(Qt::CTRL | Qt::Key_D);
    connect(m_keyCtrlD, SIGNAL(activated()), this, SLOT(generate_new_filename_from_selected_biblio_entry()));

    /*  change selected file's name */
    m_keyCtrlE = new QShortcut(this);
    m_keyCtrlE->setKey(Qt::CTRL | Qt::Key_E);
    connect(m_keyCtrlE, SIGNAL(activated()), this, SLOT( ()));

    /* change filename of selected file to proposed new name */
    m_keyCtrlC = new QShortcut(this);
    m_keyCtrlC->setKey(Qt::CTRL | Qt::Key_G);
    //connect(m_keyCtrlC, SIGNAL(activated()), this, SLOT(change_selected_filename()));

    /* search folders for pdfs */
    m_keyCtrlF = new QShortcut(this);
    m_keyCtrlF->setKey(Qt::CTRL | Qt::Key_F);
    connect(m_keyCtrlF, SIGNAL(activated()), this, SLOT(search_folders_for_pdf()));


    m_keyCtrlH = new QShortcut(this);
    m_keyCtrlH->setKey(Qt::CTRL | Qt::Key_H);
    connect(m_keyCtrlH, SIGNAL(activated()), this, SLOT(read_JSON_file_old()));

    m_keyCtrlJ = new QShortcut(this);
    m_keyCtrlJ->setKey(Qt::CTRL | Qt::Key_J);
    connect(m_keyCtrlJ, SIGNAL(activated()), this, SLOT(change_location_of_listsInfo()));

    /*  add prefix to selected file name (perhaps change biblio entries, not yet done? */
    m_keyCtrlK = new QShortcut(this);
    m_keyCtrlK->setKey(Qt::CTRL | Qt::Key_K);
    connect(m_keyCtrlK,  &QShortcut::activated, this, &Widget::add_prefix_to_selected_onboard_filename);


    /* match filestems and put in model */
    m_keyCtrlM = new QShortcut(this);
    m_keyCtrlM->setKey(Qt::CTRL | Qt::Key_M);
    connect(m_keyCtrlM, SIGNAL(activated()), this, SLOT(match_filestems() ));


    m_keyCtrlN = new QShortcut(this);
    m_keyCtrlN->setKey(Qt::CTRL | Qt::Key_N);


    m_keyCtrlO = new QShortcut(this);
    m_keyCtrlO->setKey(Qt::CTRL | Qt::Key_O);
    connect(m_keyCtrlO, SIGNAL(activated()), this, SLOT(read_JSON_file_new()));

    m_keyCtrlQ = new QShortcut(this);
    m_keyCtrlQ->setKey(Qt::CTRL | Qt::Key_Q);
    connect(m_keyCtrlQ, SIGNAL(activated()), this, SLOT(quit_now()));

    m_keyCtrlR = new QShortcut(this);
    m_keyCtrlR->setKey(Qt::CTRL | Qt::Key_R);
    connect(m_keyCtrlR, SIGNAL(activated()), this, SLOT(set_new_root_folder()));

    m_keyCtrlS = new QShortcut(this);
    m_keyCtrlS->setKey(Qt::CTRL |  Qt::Key_S);
    connect(m_keyCtrlS, SIGNAL(activated()), this, SLOT(Control_S()) ); // write_bibliography())); // context dependent meaning of Control-S

    m_keyCtrlU = new QShortcut(this);
    m_keyCtrlU->setKey(Qt::CTRL | Qt::Key_U);
    connect(m_keyCtrlU, SIGNAL(activated()), this, SLOT(add_entry_to_list()));

    m_keyCtrlZ = new QShortcut(this);
    m_keyCtrlZ->setKey(Qt::CTRL | Qt::Key_Z);
    connect(m_keyCtrlZ, SIGNAL(activated()), this, SLOT(link_biblio_entry_and_onboard_entry()));  // to do todo this should be replaced by "link_biblio_entry_and_onboard_entry"

    m_keyCtrlComma = new QShortcut(this);
    m_keyCtrlComma->setKey(Qt::CTRL | Qt::Key_Comma);
    connect(m_keyCtrlComma, SIGNAL(activated()), this, SLOT(toggle_screens_backwards()));

    m_keyCtrlPeriod = new QShortcut(this);
    m_keyCtrlPeriod->setKey(Qt::CTRL | Qt::Key_Period);
    connect(m_keyCtrlPeriod, SIGNAL(activated()), this, SLOT(toggle_screens()));
}

/*
bool Widget::biblio_model_contains(Entry* entry) {
    return m_biblio_model->contains(entry);
}
*/
void Widget::search_folders_for_pdf()
{
    delete m_onboard_pdf_model;
    m_onboard_pdf_model = new FilesModel(this);
    m_onboard_pdf_model->search_folders_for_pdfs(m_root_folder);
    m_bottom_tableView->setModel(m_onboard_pdf_model->getProxyModel());
    m_bottom_tableView->resizeColumnsToContents();


    // this is done in a different function ::  todo to do
    if (m_biblio_model){
        link_biblio_entries_and_files_by_size(); // TODO: clear out any previous linkings before doing this;
    }
}


void Widget::on_bottom_table_view_doubleClicked(QModelIndex index){
    // this way goes through the model's entry
    QModelIndex underlying_index = m_onboard_pdf_model->getProxyModel()->mapToSource(index);
    int row = underlying_index.row();
    QModelIndex folder_index = m_onboard_pdf_model->index(row,2);
    QString folder = folder_index.data().toString();
    QString file_stem = m_onboard_pdf_model->index(row,1).data().toString();
    QString filename = folder + "/" + file_stem;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}


void Widget::on_bottom_table_view_clicked(const QModelIndex& index){
    if (m_onboard_pdf_model->number_of_files() < 1) {return;}
    int row =  m_onboard_pdf_model->getProxyModel()->mapToSource( index  ).row() ;
    if (row < 0) {return;}
    // what follows is the wrong way to do it; it should be done with internal pointer and index
    QString  stem =   m_onboard_pdf_model->index(row,1).data().toString();
    QString filename = m_onboard_pdf_model->index(row,2).data().toString() + "/" + stem;
    File * file = m_onboard_pdf_model->get_file_by_full_filename(filename);
    m_selected_file = file;
    if (m_selected_file_model){
        delete m_selected_file_model;
    }

    m_selected_file_model = new FileModel(m_selected_file, this);
    m_center_tableView1_file->setModel(m_selected_file_model);

    m_selected_biblio_entry = new Entry(file);
    m_selected_entry_model = new EntryModel(m_selected_biblio_entry, m_bibliography_labels);
    m_center_tableView2_entry->setModel(m_selected_entry_model);

}

// not used: todo
void Widget::on_ToK_view_selection_changed(){
    m_selected_ToK_item =  static_cast<TreeItem*>(m_ToK_view->currentIndex().internalPointer());
    //QString key = m_selected_ToK_item->get_key();
    QString prefix  = m_selected_ToK_item->get_prefix();
    qDebug() << 398   << prefix << m_selected_ToK_item->get_string();
}
void Widget::add_ToK_item(){
    QString prefix = m_new_ToK_prefix_widget->text();
    QString title = m_new_ToK_prose_widget->text();
    m_ToK_model->addItem(prefix, title);
    m_ToK_model->dataChanged(QModelIndex(), QModelIndex());
    write_ToK_to_file();
}



void Widget::Control_S(){
    if (m_screen_state == 1) {            // the user is updating the onboard-entry to make a biblio entry...
            create_or_update_biblio_entry();
    } else{
        write_bibliography();
    }

}

void Widget::match_filestems() {
    m_entry_match_model->match_filestems();
}


/*               WIDGETS             */
Widget::~Widget()
{
    delete m_biblio_model;
   delete m_onboard_pdf_model;
}

void Widget::create_or_update_biblio_entry(){
    if (! m_biblio_model->contains(m_selected_biblio_entry) ) {
        create_new_biblio_entry();
    }
    update_selected_biblio_entry();
}

// this creates a new database Entry.
void Widget::create_new_biblio_entry(){
    if (!m_selected_biblio_entry) {return;} // todo that's not a biblio entry
    Entry * new_entry  = new Entry(m_selected_file);
    m_biblio_model->add_entry(new_entry);
    m_biblio_model->make_dirty();
}
void Widget::update_selected_biblio_entry(){
    for (int row = 0; row < m_bibliography_labels.count(); row++){
        QString label = m_bibliography_labels[row];
    }
}

QString Widget::get_selected_ToK_item_with_spaces(){
    if (!m_selected_ToK_item){return QString();}
    QString prefix = m_selected_ToK_item->get_prefix();
    prefix = prefix.split("").join(" ").trimmed() + " ";
    qDebug() << 497 << prefix;
    return prefix;
}

/* 1  User clicks on file, with intent to display it for user to add author, article name; and also to create a new biblio entry */
void Widget::put_selected_file_into_new_selected_biblio_entry(File* file, Entry* entry){
    if (entry) {delete entry;}
    entry = new Entry(file);
    m_selected_biblio_entry = entry;
}

/* 2   Now the user may add information about the file by hand, into the selected_biblio_entry model
*   That will generate a potential new name for the file
*   And the user can select it, and thereby change the name of the file on the computer.
*   Alternatively, the user can choose to simply add the tok prefix to the file name and
*   change it on the computer.
*/

/*  Control K   */
void Widget::add_prefix_to_selected_onboard_filename(){
    File* file = m_selected_file;
    if (!file) {return;}
    QString new_stem_name;
    QString old_stem_name = file->get_filename();
    QString old_full_name = file->get_folder() + "/" +  file->get_filename();
    //QFileInfo fileInfo(old_full_name);
    //QString folder= fileInfo.absolutePath();
    QString folder = file->get_folder();
    QString prefix = get_selected_ToK_item_with_spaces().trimmed();
    new_stem_name = prefix + " " + old_stem_name;
    QString new_full_name = folder +  new_stem_name;
    QFile qfile(old_full_name);
    qfile.rename(old_full_name, new_full_name);
}


void Widget::generate_new_filename_from_selected_biblio_entry(){
    QString prefix, author, author_surname;
    QString  title, year, new_filename, new_biblio_key;

    prefix = get_selected_ToK_item_with_spaces();
    year =  m_selected_biblio_entry->get_year();
    if (year.length() == 0) {
        year = QString("9999");
    }
    author = get_first_author(m_selected_biblio_entry->get_author());
    author_surname = find_surname(author);
    int max_title_length = 50;
    title = m_selected_biblio_entry->get_title();
    QString treated_title("");
    if (title.length() == 0) {
        treated_title = "title";
    } else {
        QStringList titlelist = title.split(" ");
        for (int no = 0; no < titlelist.size(); no++){
            treated_title += " " + titlelist[no];
            if (treated_title.length() >= max_title_length){
                break;
            }
        }
    }
    QString space = " ";
    QString underscore = "_";

    new_filename = prefix + space + year + space + author_surname + space + treated_title + ".pdf";
    new_biblio_key = author_surname + underscore + year + underscore + treated_title;
    m_proposed_new_title_widget->setText(new_filename);
    if (!m_biblio_model){
        qDebug() << "No biblio model currently.";
        return;
    }
    if (m_biblio_model->contains_key(new_biblio_key) ) {
        new_biblio_key += title;
    }
}




/*   was called by function change_selected_filename, but no longer used  */
void Widget::change_selected_file_filename( ){
/*    Purpose is to change the selected_file name to the string on m_proposed_new_title_widget
 *
 */ 
    File* file = m_selected_file;
    QString old_filename = file->get_filename();
    QString old_full_filename = file->get_folder() + "/" + file->get_filename();
    QString folder = file->get_folder();
    QString new_stem_name = m_proposed_new_title_widget->text();
    change_full_filename(old_full_filename, folder + "/"  + new_stem_name);
    // NB: we have not changed the biblio_entries at all.... to do
}




/*  called by "change_onboard_filename"   */
void Widget::change_full_filename(QString old_name, QString new_name){
    if (old_name == new_name){
        QMessageBox msgBox;
        msgBox.setText("The new name is the same as the old name; no change made.");
        msgBox.exec();
        return;}
    QFile file(old_name);
    file.rename(old_name, new_name);
}



/*
void Widget::update_data_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry){
    m_data_by_fullfilename.remove(full_old_name);
    m_data_by_fullfilename[full_new_name] = entry;
}
*/
void Widget::update_files_onboard_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry){
    m_files_onboard_by_filenamefull.remove(full_old_name);
    m_files_onboard_by_filenamefull[full_new_name] = entry;
}
void Widget::update_files_onboard_by_filename(QString oldfilename, QString newfilename, Entry* entry){
   // m_files_onboard_by_filename.
}

//-------------------------------------------------------------------------------------------------------
void Widget::toggle_screens_backwards(){
    m_number_of_sceen_states = 6;
    if (m_screen_state == 0){
        m_screen_state = m_number_of_sceen_states;
    } else{
        m_screen_state -= 1;
    }
    set_screen_state();
}
void Widget::toggle_screens(){
        m_number_of_sceen_states = 6;
    if (m_screen_state == m_number_of_sceen_states - 1){
        m_screen_state = 0;
    } else{
        m_screen_state += 1;
    }
    set_screen_state();
}
void Widget::set_screen_state(){
    switch ( m_screen_state ) {
        case 0:{
            m_top_tableView_biblio->setVisible(true);
            m_bottom_tableView->setVisible(true);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(true);
            m_middle_right_widget->setVisible(true);
            m_directoryView->setVisible(true);
            m_mainSplitter->setSizes(QList<int>({2000, 100, 100, 400}));
            m_mainSplitter->setStretchFactor(1,1);
            break;
        }
        case 1:{ // only biblio entries   entry view for selected item
            m_top_tableView_biblio->setVisible(true);
            m_bottom_tableView->setVisible(true);
            m_bottom_table_widget->setVisible(false);
            m_bottom_table_widget2->setVisible(false);
            //m_middle_table_wdget->setVisible(false);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(true);
            m_mainSplitter->setSizes(QList<int>({2000, 100, 1000,400}));
            m_mainSplitter->setStretchFactor(1,1);

            break;
        }
        case 2:// only onboard files and entry view for select4ed item
        {   m_bottom_tableView->setVisible(true);
            m_top_tableView_biblio->setVisible(false);
                //m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 10, 00, 400}));
            break;
        }
        case 3:{
            m_top_tableView_biblio->setVisible(true);
            m_bottom_tableView->setVisible(true);
                //m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 10, 0,400}));

            break;
        }
        case 4:{
            m_entry_match_view->setVisible(true); // not currently used, however.
            m_top_tableView_biblio->setVisible(false);
                m_bottom_tableView->setVisible(false);
                //m_middle_table_wdget->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 100, 100}));
            break;
        }
        case 5:{
            // onboard file editing
            m_rightSplitter->setVisible(true);
            m_bottom_tableView->setVisible(true);
            m_top_tableView_biblio->setVisible(false);
                //m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                //m_small_grid_layout->setVisible(false);
                m_middle_right_widget->setVisible(false);
                //m_listNamesWidget->setVisible(false);
                //m_listWidget->setVisible(false);
                m_directoryView->setVisible(false);
            break;
        }
        default:{
            m_bottom_tableView->setVisible(true);
            m_top_tableView_biblio->setVisible(true);
            //m_middle_table_wdget->setVisible(true);
            m_rightSplitter->setVisible(true);
        }
    }

}
void Widget::place_entries_with_shared_keys_on_table(){
    int row(0), rowcount(0);
    rowcount = m_biblio_model->get_count_of_multiply_used_keys();
    if (rowcount == 0) return;
    m_bottom_table_widget2->clear();
    m_bottom_table_widget2->setVisible(true);
    m_bottom_table_widget2->setColumnCount(2);
    m_bottom_table_widget2->setColumnWidth(0,400);
    m_bottom_table_widget2->setColumnWidth(1,500);
    m_bottom_table_widget2->setRowCount(rowcount);
    foreach (QString  key, m_biblio_model->get_keys_used_multiply() ) {
        QList<Entry*> entries = m_biblio_model-> get_multiple_entries_from_one_key(key);
        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_filename());
            qDebug() << 664 << entry->get_filename();
            m_bottom_table_widget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_key());
            m_bottom_table_widget2->setItem(row,1, item1);
            row++;
        }
    }
}
void Widget::place_entries_with_shared_filename_on_table(){
    int row(0), rowcount(0);
    rowcount = m_biblio_model->get_count_of_multiply_used_filenames();
    if (rowcount == 0) return;
    m_bottom_table_widget2->setRowCount(rowcount);
    m_bottom_table_widget2->setVisible(true);
    m_bottom_table_widget2->setColumnCount(2);
    m_bottom_table_widget2->setColumnWidth(0,400);
    m_bottom_table_widget2->setColumnWidth(1,500);
    foreach (QString  filename, m_biblio_model->get_list_of_filename_stems_used_muliply() ) {
        QList<Entry*> entries = m_biblio_model->get_multiple_entries_from_filename_stem(filename);
        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_author());
            qDebug() << 708 << entry->get_filename();
            m_bottom_table_widget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_filename());
            m_bottom_table_widget2->setItem(row,1, item1);
            row++;
        }
    }
}
void Widget::place_biblio_entries_with_shared_size_on_table(){
    int row(0);
    int color(0);
    int startrow(0);
    qDebug() << 718 << "check for shared size";

    int rowcount = m_biblio_model->get_count_of_multiply_used_sizes();
    /*
    foreach (int size, m_data_by_size.uniqueKeys() ){
        if (size == 0){continue;}
        QList<Entry*> entries = m_data_by_size.values(size);
        if (entries.count() > 1) { rowcount++; }
    }
    */
    if (rowcount == 0) {return;}

    m_bottom_table_widget2->setRowCount(rowcount);
    m_bottom_table_widget2->setVisible(true);
    m_bottom_table_widget2->setColumnCount(5);
    m_bottom_table_widget2->setColumnWidth(0,400);
    m_bottom_table_widget2->setColumnWidth(1,500);
    foreach (int size, m_data_by_size.uniqueKeys() ){
        if (size == 0){continue;}
        QList<Entry*> entries = m_data_by_size.values(size);
        if (entries.count() < 2) { continue; }
        startrow = row;
        foreach (Entry* entry, entries){
            QTableWidgetItem * item0 = new QTableWidgetItem (entry->get_title());
            if (row == startrow){
                if (color == 0){
                    color = 1;
                } else{
                    color = 0;
                }
            }
            if (color == 0) {
                item0->setForeground(QColorConstants::Red);
            }
            else {
                item0 ->setForeground(QColorConstants::Blue);
            }

            m_bottom_table_widget2->setItem(row,0,item0);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_author());
            m_bottom_table_widget2->setItem(row,1,item1);

            QTableWidgetItem * item2 = new QTableWidgetItem (entry->get_key());
            m_bottom_table_widget2->setItem(row,2, item2);
            QTableWidgetItem * item3 = new QTableWidgetItem (entry->get_key());
            m_bottom_table_widget2->setItem(row,3, item3);
            QTableWidgetItem * item4 = new QTableWidgetItem (QString::number(entry->get_size()));
            m_bottom_table_widget2->setItem(row,4, item4);
            row++;
        }
    }
}
QList<Entry*>  Widget::get_entries_by_size(int n){
    QList<Entry*>  list = m_data_by_size.values(n);
    return list;
}

void Widget::read_JSON_file_old()
{
}


/*                 TOP VIEW                     */

// this is not needed.
void Widget::delete_size_on_selected_biblio_entries(){
    int column_for_size = 5;
    for (int rowno = 0; rowno<m_biblio_model->rowCount(); rowno++){
        QModelIndex index = m_biblio_model->index(rowno, column_for_size);
        if (m_biblio_model->itemData(index).value(Qt::CheckStateRole) == Qt::Checked) {
            qDebug() << 1049 << "row number to be deleting size"<< rowno;
            m_biblio_model->get_entries().at(rowno)->set_size(0);
            m_biblio_model->dataChanged(index, index);
        }
    }
}
void Widget::set_new_root_folder(){
    QString foldername =  QFileDialog::getExistingDirectory(this, "Choose folder", m_root_folder);
    if(foldername.isEmpty())
        return;
    m_root_folder = foldername;
    m_settings.setValue("rootfoldername", foldername);
}
/*
void Widget::link_top_and_bottom_entries(){         // to do todo remove this? &&&
   if (! m_selected_biblio_entry) {
       qDebug() << 1301 << "Can't link entries, because no item in top view has been selected.";
       return;
   }
   if (! m_selected_onboard_entry) {\
       qDebug() << 1301 << "Can't link entries, because no item in bottom view has been selected.";
       return;
   }
   int column_for_size = 5;
   int column_for_key = 3;
   int size = m_selected_onboard_entry->get_size();

   qDebug() << 1303 << "row"<< m_entry_in_top_table << "size" << size;
   m_selected_biblio_entry->set_size(size);
   m_selected_biblio_entry->add_to_onboard_entries(m_selected_onboard_entry);
   m_selected_biblio_entry->set_filename_stem(m_selected_onboard_entry->get_filename());
   m_selected_biblio_entry->set_folder(m_selected_onboard_entry->get_folder());
   m_selected_biblio_entry->set_filename_full(m_selected_onboard_entry->get_filenamefull());
   //m_selected_biblio_entry->add_keywords(m_middle_table_wdget);
   m_selected_biblio_entry->set_info("date", m_selected_onboard_entry->get_info("date"));
   m_selected_biblio_entry->set_info("lastread", m_selected_onboard_entry->get_info("lastread"));
}
*/

void Widget::link_biblio_entry_and_file(Entry* biblio, File* file){
    biblio->add_to_onboard_entries(file);
    file->add_to_bib_entries(biblio);
    //biblio->set_filenameFull(onboard->get_filenamefull());
    biblio->set_filename_stem(file->get_filename());
    biblio->set_folder(file->get_folder());
    biblio->set_info("date_created", file->get_date_created());
    biblio->set_info("lastread", file->get_date_last_read());
    biblio->set_size(file->get_size());
}
void Widget::link_biblio_entries_and_files_by_size( ){
    if (m_biblio_model->get_entries().count() == 0 || m_onboard_pdf_model->number_of_files() == 0) {
        return;
    }
    foreach (int this_size, m_biblio_model->get_list_of_sizes_used()){
        if (this_size==0) { continue; }
        if (! m_onboard_pdf_model->contains_size(this_size)){
            continue;
        }
        if (m_biblio_model->if_size_occurs_multiply(this_size)){
            if (m_onboard_pdf_model->if_size_occurs_multiply(this_size)){
                QList<Entry*> biblios = m_biblio_model->get_multiple_entries_from_size(this_size);
                 foreach (Entry* biblio, biblios){
                     QList<File*> onboards = m_onboard_pdf_model->get_multiple_files_from_size(this_size);
                     foreach (File* file, onboards){
                         link_biblio_entry_and_file(biblio, file);
                     }
                 }
            }else{
                File* file = m_onboard_pdf_model->get_file_by_size(this_size);
                QList<Entry*> biblios = m_biblio_model->get_multiple_entries_from_size(this_size);
                foreach (Entry* biblio, biblios){
                    link_biblio_entry_and_file(biblio, file);
                }
            }
        } else{
            Entry* biblio = m_biblio_model->get_entry_by_size(this_size);
          if (m_onboard_pdf_model->if_size_occurs_multiply(this_size)){
              QList<File*>  onboards = m_onboard_pdf_model->get_multiple_files_from_size(this_size);
              foreach (File* file, onboards){
                  link_biblio_entry_and_file(biblio, file);
              }
          }  else{ // the usual case!
                File* file = m_onboard_pdf_model->get_file_by_size(this_size);
                link_biblio_entry_and_file(biblio, file);
          }
        }
    }
    emit m_biblio_model->dataChanged(QModelIndex(), QModelIndex());
}
void Widget::link_biblio_entries_and_onboard_entries_by_filename(){
    if (m_biblio_model->get_entries().count() == 0 || m_onboard_pdf_model->number_of_files() == 0) {
        return;
    }
    foreach (QString filename_stem, m_biblio_model->get_list_of_filename_stems_used() ) {

        if (! m_onboard_pdf_model->contains_filename_stem(filename_stem)){
            continue;
        }
        if (m_biblio_model->if_filename_stem_occurs_multiply(filename_stem)){
            if (m_onboard_pdf_model->if_filename_stem_occurs_multiply(filename_stem)){
                QList<Entry*> biblios = m_biblio_model->get_multiple_entries_from_filename_stem(filename_stem);
                 foreach (Entry* biblio, biblios){
                     QList<File*> onboards = m_onboard_pdf_model->get_multiple_files_from_filename_stem(filename_stem);
                     foreach (File* file, onboards){
                         link_biblio_entry_and_file(biblio, file);
                     }
                 }
            }else{
                File* file = m_onboard_pdf_model->get_file_by_filename_stem(filename_stem);
                QList<Entry*> biblios = m_biblio_model->get_multiple_entries_from_filename_stem(filename_stem);
                foreach (Entry* biblio, biblios){
                    link_biblio_entry_and_file(biblio, file);
                }
            }
        } else{
            Entry* biblio = m_biblio_model->get_entry_by_filename_stem(filename_stem);
            if (m_onboard_pdf_model->if_filename_stem_occurs_multiply(filename_stem)){
              QList<File*>  onboards = m_onboard_pdf_model->get_multiple_files_from_filename_stem(filename_stem);
              foreach (File* file, onboards){
                  link_biblio_entry_and_file(biblio, file);
              }
          }  else{ // the usual case!
                File* file = m_onboard_pdf_model->get_file_by_filename_stem(filename_stem);
                link_biblio_entry_and_file(biblio, file);
          }
        }
    }
    emit m_biblio_model->dataChanged(QModelIndex(), QModelIndex());

}



/*              MIDDLE           VIEW           */


void display_entry_on_entry_view(){

}

void display_entry_on_tablewidget(QTableWidget* table_widget, Entry* entry, QStringList & my_labels){
    table_widget->setRowCount(my_labels.length());
    table_widget->setColumnCount(4);
    table_widget->clear();
    table_widget->setColumnWidth(1,500);
    QTableWidgetItem * item;
    QStringList labels(my_labels);
    int row_number (0);
    foreach (QString label, labels){
        QString value;
        if (entry) {
            value = entry->get_info(label);
        } else { value = ""; }
        item = new QTableWidgetItem(label);
        table_widget->setItem(row_number,0,item);
        if (label == "size" && entry){
            item = new QTableWidgetItem(QString::number(entry->get_size()));
        } else{
            item = new QTableWidgetItem(value);
        }
        table_widget->setItem(row_number,1,item);
        row_number++;
        //labels.removeAll(label);
    }
}


//void Widget::put_bibitem_info_on_middle_table_widget(const Entry* entry){
//}
void Widget::on_middle_widget_item_changed(int row, int column ){
    Q_UNUSED(column);
    if (  ( m_bottom_table_widget->hasFocus()    &&
          (row == 1 || row == 2 || row == 3)  ) )
    {
        generate_new_filename_from_selected_biblio_entry();
    }
}
