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
#include <QHeaderView>
#include <EGL/egl.h>

class List;

Qt::ItemFlags BiblioTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
NewStandardItemModel::NewStandardItemModel (QObject * myparent) {
    //parent = myparent;
}

void Widget::set_screen_layout(){
    if (m_layout) delete m_layout;
    m_layout = new QVBoxLayout(this);
    if (m_mainSplitter) delete m_mainSplitter;
    m_mainSplitter = new QSplitter (Qt::Horizontal,this);
    m_layout->addWidget(m_mainSplitter);
    m_mainSplitter->setSizes(QList<int>({4000, 4000}));

    m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
    m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);
    m_mainSplitter->addWidget(m_leftSplitter);
    m_mainSplitter->addWidget(m_rightSplitter);

    m_leftSplitter->addWidget(m_topTableView);
    m_leftSplitter->addWidget(middleTableWidget);
    m_leftSplitter->addWidget(bottomTableWidget2);
    m_leftSplitter->addWidget(m_bottomTableView);
    m_leftSplitter->addWidget(m_entry_match_view);

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
       m_rightSplitter->addWidget(m_middle_right_widget);
       m_rightSplitter->addWidget(m_directoryView);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    setFocus();
    m_bibliography_labels  <<  "filenamestem" <<  "folder"<< "size"<< "author" << "title" << "year" << "key" << "fillnamefull" <<
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
    m_lists_complete_filename = m_settings.value("listsfilename").toString();


    QPalette palette = QPalette();
    palette.setColor(QPalette::Window,QColor(0,0,200,250));
    setPalette(palette);

    m_mainSplitter = nullptr;
    m_topTableView = new QTableView(this);
    middleTableWidget = new QTableWidget;
    //bottomTableWidget = new QTableWidget;
    bottomTableWidget2 = new QTableWidget;
    m_filePrefixTableWidget = new QTableWidget(this);
    m_directoryView = new QTreeView;
    myPopUp = nullptr;
    m_entry_in_bottom_table = nullptr;
    m_entry_in_middle_table = nullptr;

    onboard_pdf_model = new EntriesModel(this);
    same_size_files_model = new EntriesModel(this);
    biblioModel = new BiblioTableModel(this);
    m_entry_match_view = new EntryMatchView();

    m_topTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_topTableView->setSortingEnabled(true);

    m_bottomTableView = new EntriesView();
    m_bottomTableView->setModel(onboard_pdf_model->getProxyModel());
    m_bottomTableView->setSortingEnabled(true);
    m_bottomTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    /*
    bottomTableWidget->setColumnCount(5);
    bottomTableWidget->setColumnWidth(0,600);
    bottomTableWidget->setColumnWidth(1,250);
    bo`ttomTableWidget->setSortingEnabled(true);
    */
    bottomTableWidget2->setColumnWidth(0,1000);
    bottomTableWidget2->setColumnWidth(1,1000);
    bottomTableWidget2->setColumnWidth(3,800);
    bottomTableWidget2->setColumnWidth(4,800);
    bottomTableWidget2->setVisible(false);


    m_listWidget = new QListWidget(this);
    m_listWidget->dragEnabled();
    m_listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_listWidget->setDefaultDropAction(Qt::TargetMoveAction);
    m_listWidget->setMovement(QListView::Free);
    m_listNamesWidget = new QListWidget(this);
    m_listNamesWidget->dragEnabled();
    m_listNamesWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_listNamesWidget->setDefaultDropAction(Qt::TargetMoveAction);
    m_listNamesWidget->setMovement(QListView::Free);
    new_list("General");
    new_list("Math");
    new_list("Zellig to Noam");
    new_list("Battle");
    new_list("Bantu tone");
    new_list("Linguistica");
    new_list("Phonology");

    m_file_system_model = new QFileSystemModel(this);
    m_file_system_model->setFilter(QDir::Dirs);
    m_file_system_model->setRootPath("/home/john/");
    m_directoryView->setModel(m_file_system_model);
    m_directoryView->setColumnWidth(0,400);
    m_directoryView->setColumnWidth(1,100);
    m_directoryView->setColumnWidth(2,400);
    m_directoryView->setSortingEnabled(true);

    m_layout = nullptr;
    m_screen_state = 0;
    set_screen_layout();

    m_entry_match_model = new EntryMatchModel(biblioModel, onboard_pdf_model, this);
    //m_entry_match_view = new EntryMatchView();

    connect(m_topTableView, &TableView::clicked,
             this , &Widget::on_top_table_view_clicked);
    connect(m_topTableView, &TableView::doubleClicked,
             this , &Widget::on_top_table_view_doubleClicked);
    connect(middleTableWidget,SIGNAL(cellDoubleClicked(int,int)) ,
            this , SLOT(on_middle_table_widget_doubleClicked(int,int)));
    connect(m_delete_size_on_selected_biblio_entries,SIGNAL(clicked()) ,
            this , SLOT(delete_size_on_selected_biblio_entries())  );
      connect(m_generate_new_filename_button, &QPushButton::clicked,
            this, &Widget::generate_new_title);
    connect(m_bottomTableView,SIGNAL(cellDoubleClicked(int,int)) ,
            this , SLOT(on_bottom_table_view_doubleClicked(int,int)));
    connect(m_bottomTableView, &EntriesView::clicked,
            this, &Widget::on_bottom_table_view_clicked );
/*
    bool success = connect(bottomTableView, &TableView::currentChanged,
            this, &Widget::put_file_info_on_entry_view );
    Q_ASSERT(bool);
*/
    connect(m_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)) ,
            this , SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));
    connect(m_change_root_directory,SIGNAL(clicked()),
                this,SLOT(set_new_root_folder()));
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
    connect(m_keyCtrlS, SIGNAL(activated()), this, SLOT(write_bibliography()));

    m_keyCtrlU = new QShortcut(this);
    m_keyCtrlU->setKey(Qt::CTRL + Qt::Key_U);
    connect(m_keyCtrlU, SIGNAL(activated()), this, SLOT(add_entry_to_list()));

    m_keyCtrlZ = new QShortcut(this);
    m_keyCtrlZ->setKey(Qt::CTRL + Qt::Key_Z);
    connect(m_keyCtrlZ, SIGNAL(activated()), this, SLOT(link_top_and_bottom_entries()));

    m_keyCtrlSlash = new QShortcut(this);
    m_keyCtrlSlash->setKey(Qt::CTRL + Qt::Key_Slash);
    connect(m_keyCtrlSlash, SIGNAL(activated()), this, SLOT(toggle_screens()));

    //m_keyCtrl1 = new QShortcut(this);
    //m_keyCtrl1->setKey(Qt::CTRL + Qt::Key_1);
    //connect(m_keyCtrl1, SIGNAL(activated()), this, SLOT(toggle_right_side()));



}

void Widget::on_middle_table_widget_doubleClicked(int row,int column){
    if (! m_entry_in_middle_table ){
        qDebug() << "No entry selected on middle view";
    }
    QString filename =  m_entry_in_middle_table->get_filenamefull();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}

void Widget::match_filestems() {
    m_entry_match_model->match_filestems();
}


/*               WIDGETS             */
Widget::~Widget()
{
   delete biblioModel;
   delete onboard_pdf_model;

   //foreach(Entry * entry,    m_data_by_key){
   //   delete entry;
   // }
}
/*
void Widget::toggle_right_side(){
    if (bottomTableView->isVisible()) {
    }
    else {
        bottomTableView->setVisible(true);
        m_rightSplitter->setVisible(true);
    }
    qDebug() << 377 << bottomTableView->isVisible();
    //bottomTableView->update();
}
*/
void Widget::toggle_screens(){
    m_number_of_sceen_states = 6;
    if (m_screen_state == m_number_of_sceen_states - 1){
        m_screen_state = 0;
    } else{
        m_screen_state += 1;
    }
    switch ( m_screen_state ) {
        case 0:{
            m_topTableView->setVisible(true);
            middleTableWidget->setVisible(true);
            m_bottomTableView->setVisible(true);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(true);
            m_middle_right_widget->setVisible(true);
            m_listNamesWidget->setVisible(true);
            m_directoryView->setVisible(true);
            m_listWidget->setVisible(true);
            break;
        }
        case 1:{
            m_bottomTableView->setVisible(false);
            middleTableWidget->setVisible(false);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(false);
            break;
        }
        case 2:
        {   m_bottomTableView->setVisible(true);
            m_topTableView->setVisible(false);
            middleTableWidget->setVisible(false);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(false);
            break;
        }
        case 3:{
            m_topTableView->setVisible(true);
            m_bottomTableView->setVisible(true);
            middleTableWidget->setVisible(false);
            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(false);
            break;
        }
        case 4:{
            m_topTableView->setVisible(false);
            m_bottomTableView->setVisible(false);
            middleTableWidget->setVisible(false);
            m_entry_match_view->setVisible(true);
            m_rightSplitter->setVisible(false);
            break;
        }
        case 5:{
            m_topTableView->setVisible(false);
            middleTableWidget->setVisible(false);
            m_bottomTableView->setVisible(true);

            m_entry_match_view->setVisible(false);
            m_rightSplitter->setVisible(true);
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
            middleTableWidget->setVisible(true);
            m_rightSplitter->setVisible(true);
        }
    }

}
void Widget::load_file_prefixes(QTableWidget* table){
    int ROWCOUNT = 19;
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
    Column1 << QString("1") << QString("1") << QString("1") << QString("1")<< QString("1") << QString("1") << QString("1");
    Column2 << QString("") << QString("1") << QString("2")<< QString("3")<< QString("4")<< QString("5")<< QString("6")<< QString("7");
    Column3 << QString("Linguistics") << QString("Phonology") << QString("Morphology") << QString ("Syntax")<< QString ("History of lx")<< QString ("Semantics")<< QString ("ASL")<< QString ("African");
    Column1 << QString("1") << QString("2") << QString("2") << QString("2")<< QString("2") << QString("2") << QString("2") << QString("2");
    Column2 << QString("") << QString("1") << QString("2")<< QString("3")<< QString("4")<< QString("5")<< QString("6")<< QString("7");
    Column3 << QString("Philosophy") << QString("Philosophy of science") << QString("Philosophy of mathematics") << QString ("Continental philosophy")<< QString ("Analytic philosophy")<< QString ("Continental vs analytic")<< QString ("other history of philosophy");

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
    rowcount = biblioModel->get_count_of_multiply_used_keys();
    if (rowcount == 0) return;
    bottomTableWidget2->clear();
    bottomTableWidget2->setVisible(true);
    bottomTableWidget2->setColumnCount(2);
    bottomTableWidget2->setColumnWidth(0,400);
    bottomTableWidget2->setColumnWidth(1,500);
    bottomTableWidget2->setRowCount(rowcount);
    foreach (QString  key, biblioModel->get_keys_used_multiply() ) {
        QList<Entry*> entries = biblioModel-> get_multiple_entries_from_one_key(key);
        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_filenamestem());
            qDebug() << 664 << entry->get_filenamestem();
            bottomTableWidget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_key());
            bottomTableWidget2->setItem(row,1, item1);
            row++;
        }
    }
}
void Widget::place_entries_with_shared_filename_on_table(){
    int row(0), rowcount(0);
    rowcount = biblioModel->get_count_of_multiply_used_filenames();
    if (rowcount == 0) return;
    bottomTableWidget2->setRowCount(rowcount);
    bottomTableWidget2->setVisible(true);
    bottomTableWidget2->setColumnCount(2);
    bottomTableWidget2->setColumnWidth(0,400);
    bottomTableWidget2->setColumnWidth(1,500);
    foreach (QString  filename, biblioModel->get_filenames_used_multiply()  ){

        QList<Entry*> entries = biblioModel->get_multiple_entries_from_one_filename(filename);

        foreach (Entry* entry, entries){
            QTableWidgetItem * item = new QTableWidgetItem (entry->get_author());
            qDebug() << 708 << entry->get_filenamestem();
            bottomTableWidget2->setItem(row,0,item);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_filenamestem());
            bottomTableWidget2->setItem(row,1, item1);
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

    bottomTableWidget2->setRowCount(rowcount);
    bottomTableWidget2->setVisible(true);
    bottomTableWidget2->setColumnCount(5);
    bottomTableWidget2->setColumnWidth(0,400);
    bottomTableWidget2->setColumnWidth(1,500);
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

            bottomTableWidget2->setItem(row,0,item0);
            QTableWidgetItem * item1 = new QTableWidgetItem (entry->get_author());
            bottomTableWidget2->setItem(row,1,item1);

            QTableWidgetItem * item2 = new QTableWidgetItem (entry->get_key());
            bottomTableWidget2->setItem(row,2, item2);
            QTableWidgetItem * item3 = new QTableWidgetItem (entry->get_key());
            bottomTableWidget2->setItem(row,3, item3);
            QTableWidgetItem * item4 = new QTableWidgetItem (QString::number(entry->get_size()));
            bottomTableWidget2->setItem(row,4, item4);
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
    for (int rowno = 0; rowno<biblioModel->rowCount(); rowno++){

        QModelIndex index = biblioModel->index(rowno, column_for_size);
        if (biblioModel->itemData(index).value(Qt::CheckStateRole) == Qt::Checked) {
            qDebug() << 1049 << "row number to be deleting size"<< rowno;
            biblioModel->get_entries().at(rowno)->set_size(0);
            biblioModel->dataChanged(index, index);
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
    QStandardItem * item_top;
    QTableWidgetItem * item_bottom;
    if (m_data_by_size.size() == 0 || m_files_onboard_by_size.size() == 0) {return;}
    foreach (int this_size, m_data_by_size.uniqueKeys()){         
        if (this_size==0) { continue; }
        foreach (Entry* entry_top, m_data_by_size.values(this_size)){
            if (m_files_onboard_by_size.contains(this_size)){                              
                foreach(Entry* entry_bottom, m_files_onboard_by_size.values(this_size) ){                    
                    entry_top->add_to_onboard_entries(entry_bottom);
                    entry_bottom->add_to_bib_entries(entry_top);
                    entry_bottom->color_bottom_view_item_for_size();
                    entry_top->set_filenameFull(entry_bottom->get_filenamefull());
                    entry_top->set_filenameStem(entry_bottom->get_filenamestem());
                    entry_top->set_folder(entry_bottom->get_folder());
                    entry_top->set_info("date", entry_bottom->get_info("date"));
                    entry_top->set_info("lastread", entry_bottom->get_info("lastread"));
                }
            }
        }
      }
}

void Widget::link_top_and_bottom_entries_from_filename( ){
    QStandardItem * item_top;
    QTableWidgetItem * item_bottom;
    int temp = 0;
    foreach (QString this_filename, m_data_by_filenamestem.uniqueKeys()){
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
   m_entry_in_top_table->add_keywords(middleTableWidget);
   m_entry_in_top_table->set_info("date", m_entry_in_bottom_table->get_info("date"));
   m_entry_in_top_table->set_info("lastread", m_entry_in_bottom_table->get_info("lastread"));


}



/*              MIDDLE           VIEW           */

// this creates a new database Entry.
void Widget::create_new_bibentry(){
    QString new_name = m_proposed_new_title_widget->text();
    if (new_name.length() == 0) {return;}
    Entry* entry_onboard = m_entry_in_middle_table;
    QString author = middleTableWidget->item(3,1)->text();
    QString title =  middleTableWidget->item(4,1)->text();
    QString year =  middleTableWidget->item(5,1)->text();
    QString old_filestem = middleTableWidget->item(0,1)->text();
    QString new_keywords = middleTableWidget->item(6,1)->text();

    QString full_old_name = entry_onboard->get_filenamefull();
    QString full_new_name = entry_onboard->get_folder() + "/" + new_name;

    Entry * new_entry = new Entry();
    new_entry->set_filenameStem(new_name);
    new_entry->set_filenameFull(full_new_name);
    new_entry->set_author(author);
    new_entry->set_title(title);
    new_entry->set_year(year);
    new_entry->set_key(new_name);
    new_entry->set_keywords(new_keywords);
    new_entry->set_size(entry_onboard->get_size());
    new_entry->add_to_onboard_entries(entry_onboard);
    //update_data_by_fullfilename(full_old_name, full_new_name,entry_onboard);
    biblioModel->update_data_by_fullfilename(full_old_name, full_new_name,entry_onboard);
    update_files_onboard_by_fullfilename(full_old_name, full_new_name, entry_onboard);
    update_files_onboard_by_filenamestem(old_filestem, new_name, entry_onboard);

    biblioModel->insertRows(0,1);
    biblioModel->replace_entry(0,new_entry);
    m_topTableView->showRow(0);
    new_entry->add_to_onboard_entries(entry_onboard);
    m_files_onboard_by_filenamestem.remove(old_filestem);
    m_files_onboard_by_filenamestem.insert(new_name, new_entry);
    m_files_onboard_by_filenamefull[full_new_name] = new_entry;
    //qDebug() << 899 << entry->display();
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
void Widget::display_entry_on_middle_table(){
    Entry * entry = m_entry_in_middle_table;
    display_entry_on_tablewidget(middleTableWidget, entry, m_bibliography_labels);
}



void Widget::put_bibitem_info_on_middle_table_widget(const QModelIndex & index){
    Q_UNUSED(index);

    m_entry_in_middle_table = m_entry_in_top_table;
    display_entry_on_middle_table();
    m_proposed_new_title_widget->clear();
}

/*
void Widget::put_file_info_on_middle_table_widget(int bottom_widget_row){
    QString filename(bottomTableWidget->item(bottom_widget_row,1)->text());
    QString foldername(bottomTableWidget->item(bottom_widget_row,2)->text());
    QString fullname = foldername + "/" + filename;
    Entry* entry = m_files_onboard_by_filenamefull[fullname];
    m_entry_in_bottom_table = entry;
}
*/
void Widget::put_file_info_on_middle_table_widget(Entry* entry){
    m_entry_in_middle_table = entry;
    display_entry_on_middle_table();
    m_proposed_new_title_widget->clear();
}
void Widget::put_file_info_on_popup_widget(Entry* entry){
    if (myPopUp)
        delete myPopUp;
    myPopUp = new PopUp(entry, this);
}
void Widget::put_file_info_on_entry_view(QModelIndex & current_model_index){


}
void Widget::put_bibitem_info_on_middle_table_widget(const Entry* entry){

}
void Widget::on_middle_widget_item_changed(int row, int column ){
    Q_UNUSED(column);
    if (  ( bottomTableWidget->hasFocus() || middleTableWidget->hasFocus())  &&
          (row == 1 || row == 2 || row == 3)  )
    {
        generate_new_title();
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
void Widget::generate_new_title(){
    QString prefix;
    int prefix_row;
    if (m_filePrefixTableWidget->selectedItems().count() > 0){
        prefix_row = m_filePrefixTableWidget->selectedItems().first()->row();
        prefix = m_filePrefixTableWidget->item(prefix_row,0)->text() + " " +
                 m_filePrefixTableWidget->item(prefix_row,1)->text() + " ";
    }
    QString author_surname;
    QString  title, new_title, year, new_filename, new_biblio_key;
    int row_for_biblio_key = 3;
    if (middleTableWidget->item(0,1)) {
        author_surname = find_surname(get_first_author(middleTableWidget->item(0,1)->text()));
    }
    new_biblio_key = author_surname;
    int max_title_length = 50;
    if (middleTableWidget->item(1,1)){
        title = middleTableWidget->item(1,1)->text();
        if (title.length() == 0) {
            new_title = "title";}
        else{
            QStringList titlelist = title.split(" ");
            for (int no = 0; no < titlelist.size(); no++){
                new_title += "_" + titlelist[no];
                if (new_title.length() >= max_title_length){
                    break;
                }
            }
        }
    }
    if (middleTableWidget->item(2,1)){
        year = middleTableWidget->item(2,1)->text();
        if (year.length() == 0) {year = QString("9999");}
        new_biblio_key += "_" + year;
    }
    if (m_filePrefixTableWidget->selectedItems().count() > 0){
        int row = m_filePrefixTableWidget->selectedItems().at(0)->row();
        qDebug() << 1625 << "selected row number" << row;
    }
    if (year.length() > 0) { new_filename = year + "_";}
    if (author_surname.length() > 0) { new_filename += author_surname; }
    new_filename +=  new_title;
    new_filename = prefix + new_filename;
    m_proposed_new_title_widget->setText(new_filename + ".pdf");
    if (biblioModel->contains_key(new_biblio_key) ) { //  })  m_data_by_key.contains(new_biblio_key)){
        new_biblio_key += title;
    }
    QTableWidgetItem * item = new QTableWidgetItem(new_biblio_key);
    middleTableWidget->setItem(row_for_biblio_key,1,item);

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
            m_mainSplitter->setSizes(QList<int>({4000, 4000}));
            m_leftSplitter = new QSplitter(Qt::Vertical, m_mainSplitter);
            m_rightSplitter = new QSplitter(Qt::Vertical,m_mainSplitter);

            m_mainSplitter->addWidget(m_leftSplitter);
            m_mainSplitter->addWidget(m_rightSplitter);

            m_leftSplitter->addWidget(m_topTableView);
            m_leftSplitter->addWidget(middleTableWidget);
            m_leftSplitter->addWidget(bottomTableWidget2);
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
