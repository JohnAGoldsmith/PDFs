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
#include "BiblioTableModel.h"
#include "widget.h"

class List;

Qt::ItemFlags BiblioTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
//NewStandardItemModel::NewStandardItemModel (QObject * myparent) {
//    //parent = myparent;
//}

void Widget::set_screen_layout(){
    myPopUp = nullptr;

    if (m_layout) delete m_layout;
    m_layout = new QVBoxLayout(this);
    if (m_mainSplitter) delete m_mainSplitter;
    m_mainSplitter = new QSplitter (Qt::Horizontal,this);
    m_layout->addWidget(m_mainSplitter);

    // top level
    m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
    m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);

    m_mainSplitter->addWidget(m_leftSplitter);
    m_mainSplitter->addWidget(m_center_entry_view);
    m_mainSplitter->addWidget(m_rightSplitter);

    //m_mainSplitter->`(QList<int>({100, 50, 100}));


    // left column
    m_leftSplitter->addWidget(m_topTableView);
    m_leftSplitter->addWidget(m_middle_table_wdget);
    m_leftSplitter->addWidget(m_bottom_table_widget2);
    m_leftSplitter->addWidget(m_bottomTableView);
    m_leftSplitter->addWidget(m_entry_match_view);

    // right column
    load_file_prefixes(m_filePrefixTableWidget);
    m_filePrefixTableWidget->setSelectionMode( QAbstractItemView::SingleSelection );

    m_small_grid_layout   = new QGridLayout;
    m_middle_right_widget = new QWidget(this);
    m_middle_right_widget->setLayout(m_small_grid_layout);

       m_proposed_new_title_label = new QLabel("Proposed new title") ;
       m_proposed_new_title_widget = new QLineEdit();
       m_new_list_name_widget = new QLineEdit("(enter name of new list here)");
       m_generate_new_filename_button = new QPushButton("Generate new filename and key");
            //m_change_filename_button = new QPushButton("Change file name to (^K):");
       m_create_new_bibentry_button = new QPushButton("Create new biblio entry (^K):");
       m_change_root_directory = new QPushButton("Change root directory");
       m_create_new_list_button = new QPushButton("Create new list (^N):");
       m_save_biblio_file_button = new QPushButton("Save biblio file (^S)");
       m_add_to_list_button = new QPushButton("Add entry to selected list (^U)");
       m_link_two_entries = new QPushButton("Link top and bottom entries (^Z)");

       m_delete_selected_files_button = new QPushButton("Delete selected files");
       m_delete_size_on_selected_biblio_entries = new QPushButton("Delete size on selected biblio entries");
       m_check_biblio_for_shared_key_button = new QPushButton("Check biblio for shared keys");
       m_check_biblio_for_shared_size_button = new QPushButton("Check biblio for shared sizes");
       m_check_biblio_for_shared_filename_button = new QPushButton("Check biblio for shared filenames");

       m_small_grid_layout->addWidget(m_create_new_list_button,0,0);
       m_small_grid_layout->addWidget(m_new_list_name_widget,0,1);
       //small_grid_layout->addWidget(m_change_filename_button,1,0);
       m_small_grid_layout->addWidget(m_create_new_bibentry_button,1,0);
       m_small_grid_layout->addWidget(m_proposed_new_title_widget,1,1);
       m_small_grid_layout->addWidget(m_generate_new_filename_button,2,0);
       m_small_grid_layout->addWidget(m_delete_size_on_selected_biblio_entries,3,0);
       m_small_grid_layout->addWidget(m_change_root_directory,4,0);
       m_small_grid_layout->addWidget(m_save_biblio_file_button,5,0);
       m_small_grid_layout->addWidget(m_add_to_list_button,6,0);
       m_small_grid_layout->addWidget(m_link_two_entries,7,0);
       m_small_grid_layout->addWidget(m_delete_selected_files_button,8,0);
       m_small_grid_layout->addWidget(m_check_biblio_for_shared_key_button,9,0);
       m_small_grid_layout->addWidget(m_check_biblio_for_shared_size_button,10,0);
       m_small_grid_layout->addWidget(m_check_biblio_for_shared_filename_button,11,0);
       m_current_list = nullptr;

   m_rightSplitter->addWidget(m_listNamesWidget);
   m_rightSplitter->addWidget(m_listWidget);
   m_rightSplitter->addWidget(m_filePrefixTableWidget);
   m_rightSplitter->addWidget(m_ToK_view);
   m_rightSplitter->addWidget(m_middle_right_widget);
   m_rightSplitter->addWidget(m_directoryView);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    showMaximized();
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    setFocus();
    m_isDirty = false;


    m_bibliography_labels  <<  "filenamestem" <<  "folder"<< "size"<< "author" << "title" << "year" << "key" << "fillnamefull"
                               "type"<< "booktitle"<< "month" << "organization" << "series" << "number" << "volume" << "doi" <<
                               "file (not found)" << "issn"<< "isbn" <<  "url" << "pages" << "abstract" <<  "address" <<
                               "annote" << "mendeley-tags" << "publisher" << "journal"  << "editor" << "school" << "type" <<
                               "archiveprefix" << "arxivid" << "pmid" << "eprint" << "possible file" << "pmid" << "edition" <<
                               "primaryclass" << "chapter" << "institution" << "howpublished" << "candidates" << "file" << "translator" <<
                               "date_entry_created";
    m_bibliography_short_labels  << "author" << "title" << "year" << "key" << "filenamestem" << "size" << "folder" << "keywords";
    m_prefered_location = "Dropbox/library/";

    QSettings settings ("JohnAGoldsmith", "PdfManager");
    m_root_folder = m_settings.value("rootfoldername", "/home/john/Dropbox/").toString();
    m_json_folder = m_settings.value("jsonfoldername", "/home/").toString();
    m_init_folder = m_settings.value("initfoldername", "/home/").toString();
    m_lists_complete_filename = m_settings.value("listsfilename").toString();

    QPalette palette = QPalette();
    palette.setColor(QPalette::Window,QColor(0,0,200,250));
    setPalette(palette);

    m_mainSplitter = nullptr;
    m_selected_entry = nullptr;

    // Table widgets, which we will probably get rid of
    // left side
    m_middle_table_wdget = new QTableWidget;
    m_bottom_table_widget = new QTableWidget;
    m_bottom_table_widget2 = new QTableWidget;

    m_center_entry_view = new QTableView();
    m_center_entry_view->horizontalHeader()->setStretchLastSection(QHeaderView::Stretch);
    m_center_entry_view->setMaximumWidth(1400);
    m_center_entry_view->setMinimumWidth(500);


    m_filePrefixTableWidget = new QTableWidget(this);
    m_filePrefixTableWidget->setMinimumWidth(200);
    m_filePrefixTableWidget->setMaximumWidth(500);
    m_filePrefixTableWidget->setColumnWidth(0,40);
    m_filePrefixTableWidget->setColumnWidth(1,40);
    m_directoryView = new QTreeView;

    // for the selected entry
    m_selected_entry_model  = new EntryModel(new Entry(), get_bibliography_labels());
    m_center_entry_view->setModel(m_selected_entry_model);
    m_center_entry_view->setColumnWidth(0,160);

    // Models
    onboard_pdf_model = new EntriesModel(this);
    same_size_files_model = new EntriesModel(this);
    m_biblioModel = new BiblioTableModel(this);
    m_ToK_model = new ToK_model(this);
    read_ToK_from_json("pdf_manager_tok_init.json");

    // Views
    m_topTableView = new QTableView(this);
    m_topTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_topTableView->setSortingEnabled(true);

    m_bottomTableView = new EntriesView_onboard_files();
    m_bottomTableView->setModel(onboard_pdf_model->getProxyModel());
    m_bottomTableView->setColumnHidden(0,true);
    m_bottomTableView->setColumnWidth(1,200);
    m_entry_match_view = new EntryMatchView();

    // More table widgets, which we will get rid of:
    m_bottom_table_widget2->setColumnWidth(0,1000);
    m_bottom_table_widget2->setColumnWidth(1,1000);
    m_bottom_table_widget2->setColumnWidth(3,800);
    m_bottom_table_widget2->setColumnWidth(4,800);
    m_bottom_table_widget2->setVisible(false);

    // Lists:
    m_listWidget = new QListWidget(this);
    list_functionality();

    m_file_system_model = new QFileSystemModel(this);
    m_file_system_model->setFilter(QDir::Dirs);
    m_file_system_model->setRootPath("/home/john/");
    m_ToK_view = new QTreeView(this);
    m_ToK_view->setModel(m_ToK_model);
    m_ToK_view->expandAll();
    m_directoryView->setModel(m_file_system_model);
    m_directoryView->setColumnWidth(0,400);
    m_directoryView->setColumnWidth(1,100);
    m_directoryView->setColumnWidth(2,400);
    m_directoryView->setSortingEnabled(true);

    m_layout = nullptr;
    m_screen_state = 0;
    set_screen_layout();

    m_entry_match_model = new EntryMatchModel(m_biblioModel, onboard_pdf_model, this);

    // Selection changed signal
    //connect(m_bottomTableView, &QAbstractItemView::selectionChanged,
    //        this, &Widget::on_top_table_view_clicked);

    // Single clicks
    connect(m_topTableView, &QTableView::clicked,
             this , &Widget::on_top_table_view_clicked);
    connect(m_bottomTableView, &EntriesView::clicked,
            this, &Widget::on_bottom_table_view_clicked );
    connect(m_generate_new_filename_button, &QPushButton::clicked,
            this, &Widget::generate_new_filename);

    connect(m_delete_size_on_selected_biblio_entries,SIGNAL(clicked()) ,
            this , SLOT(delete_size_on_selected_biblio_entries())  );
    //connect(m_change_filename_button,SIGNAL(clicked()),
    //            this,SLOT(change_filename()));
    connect(m_create_new_bibentry_button,SIGNAL(clicked()),
            this,SLOT(create_new_bibentry()));
    connect(m_listNamesWidget,SIGNAL(itemClicked(QListWidgetItem*)),
                this,SLOT(select_new_list(QListWidgetItem*)));
    connect(m_create_new_list_button,SIGNAL(clicked()),
                this,SLOT(new_list()));
    connect(m_save_biblio_file_button,SIGNAL(clicked()),
                this,SLOT(write_bibliography()));
    connect(m_link_two_entries,SIGNAL(clicked()),
                this,SLOT(link_top_and_bottom_entries()));
    connect(m_add_to_list_button,SIGNAL(clicked()),
                this,SLOT(add_entry_to_list()));
    connect(m_delete_selected_files_button,SIGNAL(clicked()),
                this,SLOT(delete_selected_files()));
    connect(m_check_biblio_for_shared_key_button,SIGNAL(clicked()),
                this,SLOT(place_entries_with_shared_keys_on_table()));
    connect(m_check_biblio_for_shared_filename_button,SIGNAL(clicked()),
                this,SLOT(place_entries_with_shared_filename_on_table()));
    connect(m_check_biblio_for_shared_size_button,SIGNAL(clicked()),
                this,SLOT(place_entries_with_shared_size_on_table()));

    // Double clicks
    connect(m_topTableView, &QTableView::doubleClicked,
             this , &Widget::on_top_table_view_doubleClicked);
    //connect(m_middle_table_wdget,SIGNAL(cellDoubleClicked(int,int)) ,
    //        this , SLOT(on_middle_table_widget_doubleClicked(int,int)));
    //    connect(m_bottomTableView,SIGNAL(cellDoubleClicked(int,int)) ,
    //            this , SLOT(on_bottom_table_view_doubleClicked(int,int)));
    connect(m_bottomTableView, &QAbstractItemView::doubleClicked,
            this, &Widget::on_bottom_table_view_doubleClicked);
    connect(m_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)) ,
            this , SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));


/*
    bool success = connect(bottomTableView, &TableView::currentChanged,
            this, &Widget::put_file_info_on_entry_view );
    Q_ASSERT(bool);
*/
    connect(m_change_root_directory,SIGNAL(clicked()),
                this,SLOT(set_new_root_folder()));

    m_keyCtrlLeftBracket = new QShortcut(this);
    m_keyCtrlLeftBracket->setKey(Qt::CTRL  + Qt::Key_BracketLeft);
    connect(m_keyCtrlLeftBracket, SIGNAL(activated()), this, SLOT(open_popUp()));
    m_keyCtrlRightBracket = new QShortcut(this);
    m_keyCtrlRightBracket->setKey(Qt::CTRL  + Qt::Key_BracketRight);
    connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(close_popUp()));


    m_keyCtrlA = new QShortcut(this);
    m_keyCtrlA->setKey(Qt::CTRL + Qt::Key_A);
    connect(m_keyCtrlA, SIGNAL(activated()), this, SLOT(show_files_with_same_size()));

    m_keyCtrlC = new QShortcut(this);
    m_keyCtrlC->setKey(Qt::CTRL + Qt::Key_G);
    connect(m_keyCtrlC, SIGNAL(activated()), this, SLOT(change_selected_filename()));

    m_keyCtrlF = new QShortcut(this);
    m_keyCtrlF->setKey(Qt::CTRL + Qt::Key_F);
    connect(m_keyCtrlF, SIGNAL(activated()), this, SLOT(search_folders_for_pdf()));


    m_keyCtrlH = new QShortcut(this);
    m_keyCtrlH->setKey(Qt::CTRL + Qt::Key_H);
    connect(m_keyCtrlH, SIGNAL(activated()), this, SLOT(read_JSON_file_old()));

    m_keyCtrlJ = new QShortcut(this);
    m_keyCtrlJ->setKey(Qt::CTRL + Qt::Key_J);
    connect(m_keyCtrlJ, SIGNAL(activated()), this, SLOT(change_location_of_listsInfo()));

    m_keyCtrlK = new QShortcut(this);
    m_keyCtrlK->setKey(Qt::CTRL + Qt::Key_K);
    connect(m_keyCtrlK, SIGNAL(activated()), this, SLOT(change_filename()));


    m_keyCtrlM = new QShortcut(this);
    m_keyCtrlM->setKey(Qt::CTRL + Qt::Key_M);
    connect(m_keyCtrlM, SIGNAL(activated()), this, SLOT(match_filestems() ));


    m_keyCtrlN = new QShortcut(this);
    m_keyCtrlN->setKey(Qt::CTRL + Qt::Key_N);
    connect(m_keyCtrlN, SIGNAL(activated()), this, SLOT(create_new_list()));

    m_keyCtrlO = new QShortcut(this);
    m_keyCtrlO->setKey(Qt::CTRL + Qt::Key_O);
    connect(m_keyCtrlO, SIGNAL(activated()), this, SLOT(read_JSON_file_new()));

    m_keyCtrlQ = new QShortcut(this);
    m_keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q);
    connect(m_keyCtrlQ, SIGNAL(activated()), this, SLOT(quit_now()));

    m_keyCtrlR = new QShortcut(this);
    m_keyCtrlR->setKey(Qt::CTRL + Qt::Key_R);
    connect(m_keyCtrlR, SIGNAL(activated()), this, SLOT(set_new_root_folder()));

    m_keyCtrlS = new QShortcut(this);
    m_keyCtrlS->setKey(Qt::CTRL + Qt::Key_S);
    connect(m_keyCtrlS, SIGNAL(activated()), this, SLOT(Control_S()) ); // write_bibliography())); // context dependent meaning of Control-S

    m_keyCtrlU = new QShortcut(this);
    m_keyCtrlU->setKey(Qt::CTRL + Qt::Key_U);
    connect(m_keyCtrlU, SIGNAL(activated()), this, SLOT(add_entry_to_list()));

    m_keyCtrlZ = new QShortcut(this);
    m_keyCtrlZ->setKey(Qt::CTRL + Qt::Key_Z);
    connect(m_keyCtrlZ, SIGNAL(activated()), this, SLOT(link_top_and_bottom_entries()));

    /*
    m_keyCtrlSlash = new QShortcut(this);
    m_keyCtrlSlash->setKey(Qt::CTRL + Qt::Key_Slash);
    connect(m_keyCtrlSlash, SIGNAL(activated()), this, SLOT(toggle_screens()));
    */

    m_keyCtrlComma = new QShortcut(this);
    m_keyCtrlComma->setKey(Qt::CTRL + Qt::Key_Comma);
    connect(m_keyCtrlComma, SIGNAL(activated()), this, SLOT(toggle_screens_backwards()));

    m_keyCtrlPeriod = new QShortcut(this);
    m_keyCtrlPeriod->setKey(Qt::CTRL + Qt::Key_Period);
    connect(m_keyCtrlPeriod, SIGNAL(activated()), this, SLOT(toggle_screens()));


    //m_keyCtrl1 = new QShortcut(this);
    //m_keyCtrl1->setKey(Qt::CTRL + Qt::Key_1);
    //connect(m_keyCtrl1, SIGNAL(activated()), this, SLOT(toggle_right_side()));
}

bool Widget::biblio_model_contains(Entry* entry) {
    return m_biblioModel->contains(entry);
}
void Widget::on_bottom_table_view_doubleClicked(QModelIndex index){



    QModelIndex underlying_index = onboard_pdf_model->m_proxyModel->mapToSource(index);
    int model_row = underlying_index.row();
    qDebug() <<63   <<  index << "index row"<<index.row() << "model row" <<model_row ;
    Entry * entry = onboard_pdf_model->get_entries()->at(model_row);

    QString filename =  entry->get_filenamefull();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

    //QString filename =  m_entry_in_middle_table->get_filenamefull();
    //QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}

void Widget::on_middle_table_widget_doubleClicked(int row,int column){
    if (! m_entry_in_middle_table ){
        qDebug() << "No entry selected on middle view";
    }
    QString filename =  m_entry_in_middle_table->get_filenamefull();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

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
   delete m_biblioModel;
   delete onboard_pdf_model;
}

void Widget::create_or_update_biblio_entry(){
    if (! m_biblioModel->contains(m_selected_entry) ) {
        create_new_biblio_entry();
    }
    update_selected_biblio_entry();
}

void Widget::create_new_bibentry(){
 // TODO change this to biblio_entry...
}

// this creates a new database Entry.
void Widget::create_new_biblio_entry(){
    if (!m_selected_entry) {return;}
    Entry * new_entry  = new Entry(*m_selected_entry);
    m_biblioModel->add_entry(new_entry);
    m_biblioModel->make_dirty();
}
void Widget::update_selected_biblio_entry(){
    for (int row = 0; row < m_bibliography_labels.count(); row++){
        QString label = m_bibliography_labels[row];
        //m_selected_entry->set_info(label,  )
    }
    /*
    Entry* new_entry, *entry_onboard;
    //QString new_keywords = m_middle_table_wdget->item(6,1)->text();
    QString full_old_name = entry_onboard->get_filenamefull();
    QString full_new_name = entry_onboard->get_folder() + "/" + new_name;

    new_entry->set_filenameStem(m_proposed_new_title_widget->text());
    //new_entry->set_filenameFull(full_new_name);
    //new_entry->set_author(author);
    //new_entry->set_title(title);
    //new_entry->set_year(year);
    //new_entry->set_key(new_name);
    //new_entry->set_keywords(new_keywords);
    //new_entry->set_size(entry_onboard->get_size());
    new_entry->add_to_onboard_entries(entry_onboard);
    //update_data_by_fullfilename(full_old_name, full_new_name,entry_onboard);
    m_biblioModel->update_data_by_fullfilename(full_old_name, full_new_name,entry_onboard);
    update_files_onboard_by_fullfilename(full_old_name, full_new_name, entry_onboard);
    update_files_onboard_by_filenamestem(old_filestem, new_name, entry_onboard);

    m_biblioModel->insertRows(0,1);
    m_biblioModel->replace_entry(0,new_entry);
    m_topTableView->showRow(0);
    new_entry->add_to_onboard_entries(entry_onboard);
    m_files_onboard_by_filenamestem.remove(old_filestem);
    m_files_onboard_by_filenamestem.insert(new_name, new_entry);
    m_files_onboard_by_filenamefull[full_new_name] = new_entry;
    //qDebug() << 899 << entry->display();
    */
}
void Widget::change_onboard_filename(QString new_full_filename){
/*    Purpose is to change the filename on computer ("onboard") when it is changed in either
            the middle, the bottom widget, or the PopUp.
           It is intended for changing the filename on the computer, not in the biblio database.
           There can be several copies of the same file on the computer, and it will change the names of
            those files that have the same filenamestem.

            i. change the filename on the computer
            ii. change the filename in the Entry;
            iii. update hashes from filename to entry
            iv.   change the name on bottom widgets' Item

            v.  change the name on the middle widget.
            Vi.  if there is a matching record already, change the filename in the database too.
      The process should be triggered by focus leaving either the middle or the bottom widget  item, if it is changed.
 */
    // Verify that selected entry is a member of the onboard group:
    if (!onboard_pdf_model->contains( m_selected_entry) ) {
        return;
    }
    //(i.)
    Entry* entry = m_entry_in_bottom_table;
    QString old_filename_stem = entry->get_filenamestem();
    QString old_full_filename = entry->get_filenamefull();
    QFileInfo fileInfo(QFile(old_full_filename));
    change_filename(old_full_filename, new_full_filename);
    //(ii.)
    entry->set_filenameFull(new_full_filename);
    QFileInfo fileInfo2(new_full_filename);
    QString new_filename_stem = fileInfo2.fileName();
    entry->set_filenameStem(new_filename_stem);
    // iii.
    update_files_onboard_by_fullfilename(old_full_filename, new_full_filename, entry );
    update_files_onboard_by_filenamestem(old_filename_stem, new_filename_stem, entry );
    // iv.
    set_filename_item_bottom_widget(m_selected_row_in_bottom_table, new_filename_stem);
}

void Widget::change_filename(QString old_name, QString new_name){
    if (old_name == new_name){
        QMessageBox msgBox;
        msgBox.setText("The new name is the same as the old name; no change made.");
        msgBox.exec();
        return;}
    QFile file(old_name);
    file.rename(old_name, new_name);
}
void Widget::change_selected_filename(){
    QString new_filename;
    Entry* entry = m_entry_in_bottom_table;
    //change_onboard_filename()

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
void Widget::update_files_onboard_by_filenamestem(QString oldfilename, QString newfilename, Entry* entry){
   // m_files_onboard_by_filenamestem.
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
            m_topTableView->setVisible(true);
            m_middle_table_wdget->setVisible(true);
            m_bottomTableView->setVisible(true);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(true);
            m_middle_right_widget->setVisible(true);
            m_listNamesWidget->setVisible(true);
            m_directoryView->setVisible(true);
            m_listWidget->setVisible(true);
            m_mainSplitter->setSizes(QList<int>({2000, 100, 1000}));
            break;
        }
        case 1:{

            m_topTableView->setVisible(true);
            m_bottomTableView->setVisible(false);
            m_middle_table_wdget->setVisible(false);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 10, 00}));

            break;
        }
        case 2:
        {   m_bottomTableView->setVisible(true);
                m_topTableView->setVisible(false);
                m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 10, 00}));
            break;
        }
        case 3:{
            m_topTableView->setVisible(true);
            m_bottomTableView->setVisible(true);
                m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 10, 0}));

            break;
        }
        case 4:{
            m_entry_match_view->setVisible(true);
                m_topTableView->setVisible(false);
                m_bottomTableView->setVisible(false);
                m_middle_table_wdget->setVisible(false);
                m_rightSplitter->setVisible(false);
            m_mainSplitter->setSizes(QList<int>({1000, 100, 100}));
            break;
        }
        case 5:{
            // onboard file editing
            m_rightSplitter->setVisible(true);
            m_bottomTableView->setVisible(true);
                m_topTableView->setVisible(false);
                m_middle_table_wdget->setVisible(false);
                m_entry_match_view->setVisible(false);
                //m_small_grid_layout->setVisible(false);
                m_middle_right_widget->setVisible(false);
                m_listNamesWidget->setVisible(false);
                m_listWidget->setVisible(false);
                m_directoryView->setVisible(false);
            break;
        }
        default:{
            m_bottomTableView->setVisible(true);
            m_topTableView->setVisible(true);
            m_middle_table_wdget->setVisible(true);
            m_rightSplitter->setVisible(true);
        }
    }

}
void Widget::load_file_prefixes(QTableWidget* table){
    int ROWCOUNT = 55;
    table->setColumnCount(3);
    //table->setColumnWidth(0,6);
    //table->setColumnWidth(1,6);
    //table->setColumnWidth(2,200);
    // Disable scroll bar of the table
    table->horizontalScrollBar()->setDisabled(true);
    //table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //table->setHorizontalScrollBarPolicy(AdjustIgnored);
    table->horizontalHeader()->setStretchLastSection(true);

    table->setRowCount(ROWCOUNT);
    QStringList Column1, Column2, Column3;
    Column1 << QString("0") << QString("0") <<  QString("0") << QString("0") ;
    Column2 << QString("0") << QString("1") << QString("2") << QString("3");
    Column3 << QString("Zellig2Noam") << QString("Linguistica and segmentation") << QString("Prime suspect") << QString ("Battle in the Mind Fields");
    Column1 << QString("1") << QString("1") << QString("1") << QString("1")<< QString("1") << QString("1") << QString("1") << QString("1");
    Column2 << QString("") << QString("1") << QString("2")<< QString("3")<< QString("4")<< QString("5")<< QString("6")<< QString("7");
    Column3 << QString("Linguistics") << QString("Phonology") << QString("Morphology") << QString ("Syntax")<< QString ("History of lx")<< QString ("Semantics")<< QString ("ASL")<< QString ("African");
    Column1 << QString("2") << QString("2") << QString("2") << QString("2")<< QString("2") << QString("2") << QString("2");
    Column2 << QString("") << QString("1") << QString("2")<< QString("3")<< QString("4")<< QString("5")<< QString("6");
    Column3 << QString("Philosophy") << QString("Philosophy of science") << QString("Philosophy of mathematics") << QString ("Continental philosophy")<< QString ("Analytic philosophy")<< QString ("Continental vs analytic")<< QString ("other history of philosophy");
    Column1 << QString("3") << QString("3") << QString("3") ;
    Column2 << QString("") << QString("1") << QString("2") ;
    Column3 << QString("Psychology") << QString("cognitive psychology") << QString("history of psychology");
    Column1 << QString("4") << QString("4") << QString("4") << QString("4") << QString("4") << QString("4") ;
    Column2 << QString("") << QString("1") << QString("2") << QString("3") << QString("4") << QString("5") ;
    Column3 << QString("Computer science") << QString("Machine learning") << QString("Data visualization")
            << QString("Programming") << QString("Comp Lx NLP") << QString("Algorithmic complexity");
    Column1 << QString("5") << QString("5") << QString("5") << QString("5") << QString("5") << QString("5")
               << QString("5") << QString("5") << QString("5");
    Column2 << QString("") << QString("1") << QString("2") << QString("3") << QString("4") << QString("5")<< QString("6") << QString("7") << QString("8") ;
    Column3 << QString("Mathematics") << QString("History of math") << QString("Algebra")
            << QString("Category theory") << QString("Statistics") << QString("Graphs")
            << QString("Number theory") << QString("Topology (algebraic)") << QString("Probability");
    Column1 << QString("6") << QString("6") << QString("6");
    Column2 << QString("") << QString("1") << QString("2")  ;
    Column3 << QString("Enlightenment") << QString("Sufism") << QString("Esotericism");
    Column1 << QString("7") << QString("7") << QString("7") << QString("7") << QString("7") << QString("7")
               << QString("7") ;
    Column2 << QString("") << QString("1") << QString("2") << QString("3") << QString("4") << QString("5")<< QString("6")  ;
    Column3 << QString("History and politics") << QString("European history") << QString("Drug trafficking")
            << QString("Afghanistan") << QString("Russia") << QString("Intelligence")
            << QString("France");
    Column1 << QString("8") << QString("8") << QString("8") << QString("8") ;
    Column2 << QString("") << QString("1") << QString("2") << QString("3")  ;
    Column3 << QString("Science") << QString("History of science") << QString("Physics")
            << QString("Statistical models");

    Column1 << QString("9") << QString("9") << QString("9") << QString("9") ;
    Column2 << QString("") << QString("1") << QString("2") << QString("3")  ;
    Column3 << QString("Social sciences") << QString("Economics") << QString("Anthropology")
            << QString("Sociology");


    QTableWidgetItem * item1, *item2, *item3;
    for (int row = 0; row < ROWCOUNT; row++ ){
        item1 = new QTableWidgetItem(Column1[row]);
        table->setItem(row,0,item1);
        item2 = new QTableWidgetItem(Column2[row]);
        table->setItem(row,1,item2);
        item3 = new QTableWidgetItem(Column3[row]);
        table->setItem(row,2,item3);

    }
}

void Widget::place_entries_with_shared_keys_on_table(){
    int row(0), rowcount(0);
    rowcount = m_biblioModel->get_count_of_multiply_used_keys();
    if (rowcount == 0) return;
    m_bottom_table_widget2->clear();
    m_bottom_table_widget2->setVisible(true);
    m_bottom_table_widget2->setColumnCount(2);
    m_bottom_table_widget2->setColumnWidth(0,400);
    m_bottom_table_widget2->setColumnWidth(1,500);
    m_bottom_table_widget2->setRowCount(rowcount);
    foreach (QString  key, m_biblioModel->get_keys_used_multiply() ) {
        QList<Entry*> entries = m_biblioModel-> get_multiple_entries_from_one_key(key);
        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_filenamestem());
            qDebug() << 664 << entry->get_filenamestem();
            m_bottom_table_widget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_key());
            m_bottom_table_widget2->setItem(row,1, item1);
            row++;
        }
    }
}
void Widget::place_entries_with_shared_filename_on_table(){
    int row(0), rowcount(0);
    rowcount = m_biblioModel->get_count_of_multiply_used_filenames();
    if (rowcount == 0) return;
    m_bottom_table_widget2->setRowCount(rowcount);
    m_bottom_table_widget2->setVisible(true);
    m_bottom_table_widget2->setColumnCount(2);
    m_bottom_table_widget2->setColumnWidth(0,400);
    m_bottom_table_widget2->setColumnWidth(1,500);
    foreach (QString  filename, m_biblioModel->get_filenames_used_multiply()  ){

        QList<Entry*> entries = m_biblioModel->get_multiple_entries_from_one_filename(filename);

        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_author());
            qDebug() << 708 << entry->get_filenamestem();
            m_bottom_table_widget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_filenamestem());
            m_bottom_table_widget2->setItem(row,1, item1);
            row++;
        }
    }
}
void Widget::place_entries_with_shared_size_on_table(){
    int row(0), rowcount(0);
    int color(0);
    int startrow(0);
    qDebug() << 718 << "check for shared size";

    foreach (int size, m_data_by_size.uniqueKeys() ){
        if (size == 0){continue;}
        QList<Entry*> entries = m_data_by_size.values(size);
        if (entries.count() > 1) { rowcount++; }
    }
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

/* RANDOM   */
void Widget::promote_file_from_preferred_location(Entry* entry){
    int fileno = 0;
    QString filename = "possible_file::"  + QString::number(fileno);
    while (entry->contains_info(filename)){
        QString this_filename = entry->get_info(filename);
        if (this_filename.startsWith(m_prefered_location)){
            int slash_loc = this_filename.lastIndexOf("/") + 1;
            QString foldername = this_filename.left(slash_loc);
            QString stem_name = this_filename.mid(slash_loc);
            qDebug() << 415 << this_filename << foldername << stem_name;
            if (entry->get_filenamestem().length() > 0 &&
              !(entry->get_filenamestem() == stem_name) ){
                qDebug() << 416 << "We found an occurrence of the file in the preferred folder but its stem name is different." << entry->get_filenamestem() << stem_name;
            }
            entry->set_filenameFull(this_filename);
            entry->set_filenameStem(stem_name);
            entry->set_info("folder", foldername);
            break;
        } else{
            fileno++;
        }
        filename = "possible_file::"  + QString::number(fileno);
    }
}


/*                 TOP VIEW                     */


// THis is no longer used -- the Qt internal functions are used model/view
void Widget::add_entry_to_top_view (Entry* entry){
     Q_UNUSED(entry);

}
void Widget::delete_size_on_selected_biblio_entries(){
    int column_for_size = 5;
    for (int rowno = 0; rowno<m_biblioModel->rowCount(); rowno++){

        QModelIndex index = m_biblioModel->index(rowno, column_for_size);
        if (m_biblioModel->itemData(index).value(Qt::CheckStateRole) == Qt::Checked) {
            qDebug() << 1049 << "row number to be deleting size"<< rowno;
            m_biblioModel->get_entries().at(rowno)->set_size(0);
            m_biblioModel->dataChanged(index, index);
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
void Widget::link_top_and_bottom_entries_from_size( ){
    //QStandardItem * item_top;
    //`QTableWidgetItem * item_bottom;
    if (m_data_by_size.size() == 0 || m_files_onboard_by_size.size() == 0) {return;}
    foreach (int this_size, m_data_by_size.uniqueKeys()){         
        if (this_size==0) { continue; }
        foreach (Entry* entry_top, m_data_by_size.values(this_size)){
            if (m_files_onboard_by_size.contains(this_size)){                              
                foreach(Entry* entry_bottom, m_files_onboard_by_size.values(this_size) ){                    
                    entry_top->add_to_onboard_entries(entry_bottom);
                    entry_bottom->add_to_bib_entries(entry_top);
                    entry_bottom->color_bottom_view_item_for_size();
                    //entry_top->set_filenameFull(entry_bottom->get_filenamefull());
                    entry_top->set_filenameStem(entry_bottom->get_filenamestem());
                    entry_top->set_folder(entry_bottom->get_folder());
                    entry_top->set_info("date", entry_bottom->get_info("date"));
                    entry_top->set_info("lastread", entry_bottom->get_info("lastread"));
                }
            }
        }
      }
}

/*
void Widget::link_top_and_bottom_entries_from_filename( ){
    QStandardItem * item_top;
    QTableWidgetItem * item_bottom;
    int temp = 0;
    //foreach (QString this_filename, m_data_by_filenamestem.uniqueKeys()){
    foreach (QString this_filename, biblioModel->get_keys_used_multiply(){
        foreach (Entry* entry_top, m_data_by_filenamestem.values(this_filename)) {
            foreach(Entry* entry_top_2, m_data_by_filenamestem.values(this_filename)){
                if (entry_top == entry_top_2) { continue; }
                entry_top->add_to_bib_entries(entry_top_2);
            }
            QList<Entry*> onboard_list = m_files_onboard_by_filenamestem.values(this_filename);
            foreach (Entry* entry_bottom, onboard_list){                
                entry_bottom->color_bottom_view_item_for_filename();
                entry_top->add_to_onboard_entries(entry_bottom);
                entry_bottom->add_to_bib_entries(entry_top);
            }
        }
    }
}
*/
void Widget::link_top_and_bottom_entries(){
   if (! m_entry_in_top_table) {\
       qDebug() << 1301 << "Can't link entries, because no item in top view has been selected.";
       return;
   }
   if (! m_entry_in_bottom_table) {\
       qDebug() << 1301 << "Can't link entries, because no item in bottom view has been selected.";
       return;
   }
   int column_for_size = 5;
   int column_for_key = 3;
   int size = m_entry_in_bottom_table->get_size();

   qDebug() << 1303 << "row"<< m_entry_in_top_table << "size" << size;
   m_entry_in_top_table->set_size(size);
   m_entry_in_top_table->add_to_onboard_entries(m_entry_in_bottom_table);
   m_entry_in_top_table->set_filenameStem(m_entry_in_bottom_table->get_filenamestem());
   m_entry_in_top_table->set_folder(m_entry_in_bottom_table->get_folder());
   m_entry_in_top_table->set_filenameFull(m_entry_in_bottom_table->get_filenamefull());
   m_entry_in_top_table->add_keywords(m_middle_table_wdget);
   m_entry_in_top_table->set_info("date", m_entry_in_bottom_table->get_info("date"));
   m_entry_in_top_table->set_info("lastread", m_entry_in_bottom_table->get_info("lastread"));


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
/*
void display_entry_on_tableview(QTableView* table_view, Entry* entry, QStringList & my_labels){
    //table_view->setRowCount(my_labels.length());
    //table_view->setColumnCount(4);
    //table_view->clear();
    //table_widget->setColumnWidth(1,500);
    //table_view * item;
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
*/
//void Widget::display_entry_on_middle_table(){
//    Entry * entry = m_entry_in_middle_table;
//    display_entry_on_tablewidget(m_middle_table_wdget, entry, m_bibliography_labels);/
//}
/*
void Widget::put_bibitem_info_on_middle_table_widget(const QModelIndex & index){
    Q_UNUSED(index);
    m_entry_in_middle_table = m_entry_in_top_table;
    display_entry_on_middle_table();
    m_proposed_new_title_widget->clear();
}
*/
/*
void Widget::put_file_info_on_middle_table_widget(int bottom_widget_row){
    QString filename(bottomTableWidget->item(bottom_widget_row,1)->text());
    QString foldername(bottomTableWidget->item(bottom_widget_row,2)->text());
    QString fullname = foldername + "/" + filename;
    Entry* entry = m_files_onboard_by_filenamefull[fullname];
    m_entry_in_bottom_table = entry;
}
*/
/*
void Widget::put_file_info_on_center_entry_view(Entry* entry){
    //m_entry_in_middle_table = entry;
    m_selected_entry  = entry;                                  // TODO
    m_center_entry_model->change_entry(entry);
    //display_entry_on_center_entry_view();
    //m_proposed_new_title_widget->clear();
}
*/

void Widget::put_file_info_on_entry_view(QModelIndex & current_model_index){


}
//void Widget::put_bibitem_info_on_middle_table_widget(const Entry* entry){
//}
void Widget::on_middle_widget_item_changed(int row, int column ){
    Q_UNUSED(column);
    if (  ( m_bottom_table_widget->hasFocus() || m_middle_table_wdget->hasFocus())  &&
          (row == 1 || row == 2 || row == 3)  )
    {
        generate_new_filename();
    }
}

/*              BIB MODEL               */
/*              we're moving this into the BiblioModel               */
/*
void Widget::register_biblioentry_by_key_name_and_size(Entry* entry){
    //new:
    biblioModel->register_entry(entry);
    // old:
    register_biblioentry_by_key(entry);
    register_biblioentry_by_filenamestem(entry);
    register_biblioentry_by_fullfilename(entry);
    register_biblioentry_by_size(entry);
}
*/
// remove:
/*
void Widget::register_biblioentry_by_size(Entry * entry ){
    int this_size = entry->get_size();
    if (m_data_by_size.contains(this_size)){
        //qDebug() << 1310 << entry->get_author() << entry->get_filenamestem();
    }
    m_data_by_size.insert(this_size, entry);
}
*/
//remove:
/*
void Widget::register_biblioentry_by_fullfilename(Entry* entry){
    m_data_by_fullfilename[entry->get_filenamefull()] = entry;
}
*
//remove:
void Widget::register_biblioentry_by_filenamestem(Entry* entry){
    m_data_by_filenamestem.insert(entry->get_filenamestem(), entry);
}
*/
/*
void Widget::`register_biblioentry_by_key(Entry* entry){
    QString key;
    if (entry->get_key().length() > 0 && m_data_by_key.contains(entry->get_key())){
        m_filename_collisions.insert(entry->get_key(), entry);
        qDebug() << 594 << "collision by key" << entry->get_key();
    } else{
        if (entry->get_key().length() > 0){            
            m_data_by_key[entry->get_key()] = entry;
        } else {
            key = entry->get_filenamestem();
            entry->set_key(key);
            m_data_by_key[key] = entry;
        }
    }
}
*/
void Widget::generate_new_filename(){
    QString prefix, author, author_surname;
    QString  title, year, new_filename, new_biblio_key;

    if (m_filePrefixTableWidget->selectedItems().count() > 0){
        int prefix_row = m_filePrefixTableWidget->selectedItems().first()->row();
        prefix = m_filePrefixTableWidget->item(prefix_row,0)->text() + " " +
                 m_filePrefixTableWidget->item(prefix_row,1)->text() + " ";
    }

    year =  m_selected_entry->get_year();
    if (year.length() == 0) {
        year = QString("9999");
    }

    author = get_first_author(m_selected_entry->get_author());
    author_surname = find_surname(author);

    int max_title_length = 50;
    title = m_selected_entry->get_title();
    if (title.length() == 0) {
            title = "title";
    } else {
            QStringList titlelist = title.split(" ");
            for (int no = 0; no < titlelist.size(); no++){
                title += " " + titlelist[no];
                if (title.length() >= max_title_length){
                    break;
                }
            }            
    }    
    QString space = " ";
    QString underscore = "_";

    new_filename = prefix + space + year + space + author_surname + space + title + ".pdf";
    new_biblio_key = author_surname + underscore + year + underscore + author_surname;
    m_proposed_new_title_widget->setText(new_filename);

    if (m_biblioModel->contains_key(new_biblio_key) ) {
        new_biblio_key += title;
    }
    /*
    m_selected_entry->set_filenameStem(new_filename);
    m_selected_entry->set_key(new_biblio_key);

    QModelIndex filename_index = m_selected_entry_model->index(  m_selected_entry_model->get_filename_row() ,1 );
    emit m_selected_entry_model->dataChanged(filename_index, filename_index);
    */
}

void Widget::on_listWidget_doubleClicked(QListWidgetItem* item){
    int row = m_listWidget->row(item);
    qDebug() << 724 << row;
    Entry * entry = m_current_list->get_entry(row);
    QString filename = entry->get_filenamefull();
    qDebug() << 728 << filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
    setWindowState(Qt::WindowActive);
}
/*
QString Widget::test_key_for_uniqueness (QString key){
    while (m_data_by_key.contains(key)){
        key += "@";
        qDebug() << 761 << "Collision of new key "<< key;
    }
    return key;
}
*/
List::List(QString this_name){
    name = this_name;
}
void Widget::set_screen_layout_old(){
    if (m_layout) delete m_layout;
    m_layout = new QVBoxLayout(this);

    switch (m_screen_state) {
        case 0:{
            if (m_mainSplitter) delete m_mainSplitter;
            m_mainSplitter = new QSplitter (Qt::Horizontal,this);
            m_layout->addWidget(m_mainSplitter);
            //m_mainSplitter->setSizes(QList<int>({4000, 4000}));
            m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
            m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);

            m_mainSplitter->addWidget(m_leftSplitter);
            m_mainSplitter->addWidget(m_center_entry_view);
            m_mainSplitter->addWidget(m_rightSplitter);

            m_leftSplitter->addWidget(m_topTableView);
            m_leftSplitter->addWidget(m_middle_table_wdget);
            m_leftSplitter->addWidget(m_bottom_table_widget2);
            m_leftSplitter->addWidget(m_bottomTableView);
            m_leftSplitter->addWidget(m_entry_match_view);

            load_file_prefixes(m_filePrefixTableWidget);
            m_filePrefixTableWidget->setSelectionMode( QAbstractItemView::SingleSelection );
            //QHeaderView* header = m_filePrefixTableWidget ->horizontalHeader();
            //header->setSectionResizeMode(QHeaderView::Stretch);
            //header->setStretchLastSection(true);
            //m_filePrefixTableWidget->resizeRowToContents(3);
            //m_filePrefixTableWidget->setColumnWidth(2,500);
            m_filePrefixTableWidget->horizontalHeader()->setStretchLastSection(true);

            m_middle_right_widget = new QWidget(this);
            m_small_grid_layout = new QGridLayout;
            m_middle_right_widget->setLayout(m_small_grid_layout);

            m_proposed_new_title_label = new QLabel("Proposed new title") ;
            m_proposed_new_title_widget = new QLineEdit();
            m_new_list_name_widget = new QLineEdit("(enter name of new list here)");
            m_generate_new_filename_button = new QPushButton("Generate new filename and key");
            //m_change_filename_button = new QPushButton("Change file name to (^K):");
            m_create_new_bibentry_button = new QPushButton("Create new biblio entry (^K):");
            m_change_root_directory = new QPushButton("Change root directory");
            m_create_new_list_button = new QPushButton("Create new list (^N):");
            m_save_biblio_file_button = new QPushButton("Save biblio file (^S)");
            m_add_to_list_button = new QPushButton("Add entry to selected list (^U)");
            m_link_two_entries = new QPushButton("Link top and bottom entries (^Z)");
            m_delete_selected_files_button = new QPushButton("Delete selected files");
            m_delete_size_on_selected_biblio_entries = new QPushButton("Delete size on selected biblio entries");
            m_check_biblio_for_shared_key_button = new QPushButton("Check biblio for shared keys");
            m_check_biblio_for_shared_size_button = new QPushButton("Check biblio for shared sizes");
            m_check_biblio_for_shared_filename_button = new QPushButton("Check biblio for shared filenames");

            m_small_grid_layout->addWidget(m_create_new_list_button,0,0);
            m_small_grid_layout->addWidget(m_new_list_name_widget,0,1);
            //small_grid_layout->addWidget(m_change_filename_button,1,0);
            m_small_grid_layout->addWidget(m_create_new_bibentry_button,1,0);
            m_small_grid_layout->addWidget(m_proposed_new_title_widget,1,1);
            m_small_grid_layout->addWidget(m_generate_new_filename_button,2,0);
            m_small_grid_layout->addWidget(m_delete_size_on_selected_biblio_entries,3,0);
            m_small_grid_layout->addWidget(m_change_root_directory,4,0);
            m_small_grid_layout->addWidget(m_save_biblio_file_button,5,0);
            m_small_grid_layout->addWidget(m_add_to_list_button,6,0);
            m_small_grid_layout->addWidget(m_link_two_entries,7,0);
            m_small_grid_layout->addWidget(m_delete_selected_files_button,8,0);
            m_small_grid_layout->addWidget(m_check_biblio_for_shared_key_button,9,0);
            m_small_grid_layout->addWidget(m_check_biblio_for_shared_size_button,10,0);
            m_small_grid_layout->addWidget(m_check_biblio_for_shared_filename_button,11,0);

            m_current_list = nullptr;

            m_rightSplitter->addWidget(m_listNamesWidget);
            m_rightSplitter->addWidget(m_listWidget);
            m_rightSplitter->addWidget(m_filePrefixTableWidget);
            m_rightSplitter->addWidget(m_middle_right_widget);
            m_rightSplitter->addWidget(m_directoryView);

            break;
            }
    case 1:{
        m_mainSplitter = new QSplitter (Qt::Horizontal,this);
        m_layout->addWidget(m_mainSplitter);
        m_mainSplitter->setSizes(QList<int>({4000, 4000}));
        m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
        m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);
        m_mainSplitter->addWidget(m_leftSplitter);
        m_mainSplitter->addWidget(m_rightSplitter);
        m_leftSplitter->addWidget(m_topTableView);
           break;
        }
    case 2:{
        break;
        }
    case 3:{

        break;
        }
    }
}
