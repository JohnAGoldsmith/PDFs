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


class List;

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::TableModel(QList< Entry* > entrylist, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_entries  = entrylist;
}
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_entries.size();
}
int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}
QVariant TableModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= m_entries.size() || index.row() < 0)
         return QVariant();

     if (role == Qt::DisplayRole) {
         Entry* entry   = m_entries.at(index.row());
         if (index.column() == 0){
             return entry->get_author();}
         if (index.column() == 1) {
             return entry->get_filenamestem();}
         if (index.column() == 2){
             return entry->get_folder(); }
         if (index.column() == 3){
             return entry->get_key(); }
         if (index.column() == 4){
             return entry->get_keywords(); }

     if (role == Qt::FontRole && entry->get_on_board_entries().size() > 0){
         QFont font;
         font.setBold(true);
         return font;
     }

     }
     return QVariant();
 }
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Author");
            case 1:
                return tr("Filename");
            case 2:
                return tr("Folder");
            case 3:
                return tr("Item key");
            case 4:
                return tr("Key words");


        default:
                return QVariant();
        }
    }
    return QVariant();
}
bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        //QPair<QString, QString> pair(" ", " ");
        Entry * entry = new Entry();
        m_entries.insert(position, entry);
        //listOfPairs.insert(position, pair);
    }

    endInsertRows();
    return true;
}
bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        m_entries.removeAt(position);
    }

    endRemoveRows();
    return true;
}
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        if (index.isValid() && role == Qt::EditRole) {
                int row = index.row();
                Entry * p = m_entries.value(row);
                if (index.column() == 0)
                    {p->set_author( value.toString());}
                    else {
                        if (index.column() == 2) {
                            p->set_title( value.toString() );}
                        else {
                            return false;}
                    }
                m_entries.replace(row, p);
                emit(dataChanged(index, index));
                return true;
        }
        return false;
}
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
NewStandardItemModel::NewStandardItemModel (QObject * myparent) {
    //parent = myparent;
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    setFocus();
    m_bibliography_labels  <<  "filenamestem" <<  "folder"<< "size"<< "author" << "title" << "year" << "key" << "fillnamefull" << "type"<< "booktitle"<< "month" << "organization" << "series" << "number" << "volume" << "doi" << "file (not found)"
            << "issn"<< "isbn" <<  "url" << "pages" << "abstract" <<  "address" << "annote" << "mendeley-tags" << "publisher" << "journal"  << "editor" << "school" << "type" << "archiveprefix"
            << "arxivid" << "pmid" << "eprint" << "possible file" << "pmid" << "edition" << "primaryclass" << "chapter" << "institution" << "howpublished" << "candidates" << "file" << "translator";
    m_bibliography_short_labels  << "author" << "title" << "year" << "key" << "filenamestem" << "size" << "folder" << "keywords";
    m_prefered_location = "Dropbox/library/";

    QSettings settings ("JohnAGoldsmith", "PdfManager");
    m_root_folder = m_settings.value("rootfoldername", "/home/john/Dropbox/").toString();
    m_json_folder = m_settings.value("jsonfoldername", "/home/").toString();
    m_lists_complete_filename = m_settings.value("listsfilename").toString();


    topTableView = new QTableView;
    middleTableWidget = new QTableWidget;
    bottomTableWidget = new QTableWidget;
    directoryView = new QTreeView;

    topTableView->setColumnWidth(0,1000);
    topTableView->setColumnWidth(1,1000);
    topTableView->setColumnWidth(3,800);
    topTableView->setColumnWidth(4,800);
    topTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    topTableView->setSortingEnabled(true);
    bottomTableWidget->setColumnCount(5);
    bottomTableWidget->setColumnWidth(0,600);
    bottomTableWidget->setColumnWidth(1,250);
    bottomTableWidget->setSortingEnabled(false);

    // bibModel is not used currently. Remove this.
    bibModel    = new QStandardItemModel(this);
    biblioModel = new TableModel(this);

    //topTableView->setModel(bibModel);
    //topTableView->setModel(biblioModel);
    //topTableView->show();

    listWidget = new QListWidget(this);
    listWidget->dragEnabled();
    listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    listWidget->setDefaultDropAction(Qt::TargetMoveAction);
    listWidget->setMovement(QListView::Free);
    listNamesWidget = new QListWidget(this);
    listNamesWidget->dragEnabled();
    listNamesWidget->setDragDropMode(QAbstractItemView::InternalMove);
    listNamesWidget->setDefaultDropAction(Qt::TargetMoveAction);
    listNamesWidget->setMovement(QListView::Free);
    new_list("General");
    new_list("Math");
    new_list("Zellig to Noam");
    new_list("Battle");
    new_list("Bantu tone");
    new_list("Linguistica");
    new_list("Phonology");

    QFileSystemModel * model = new QFileSystemModel;
    model->setFilter(QDir::Dirs);
    model->setRootPath("/home/john/");
    directoryView->setModel(model);
    directoryView->setColumnWidth(0,400);
    directoryView->setColumnWidth(1,100);
    directoryView->setColumnWidth(2,400);
    directoryView->setSortingEnabled(true);

    // this should be deleted:
    QGridLayout * mainLayout = new QGridLayout;






    QVBoxLayout* layout = new QVBoxLayout(this);
    QSplitter *  mainSplitter = new QSplitter (Qt::Horizontal,this);
    layout->addWidget(mainSplitter);
    mainSplitter->setSizes(QList<int>({4000, 4000}));
    QSplitter *  leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
    QSplitter *  rightSplitter = new QSplitter(Qt::Vertical,mainSplitter);
    mainSplitter->addWidget(leftSplitter);
    mainSplitter->addWidget(rightSplitter);
    leftSplitter->addWidget(topTableView);
    leftSplitter->addWidget(middleTableWidget);
    leftSplitter->addWidget(bottomTableWidget);
    rightSplitter->addWidget(listNamesWidget);
    rightSplitter->addWidget(listWidget);







    // all these mainLayout should be deleted:
    mainLayout->addWidget(topTableView,0,0);
    mainLayout->setRowMinimumHeight(3,0);
    mainLayout->addWidget(middleTableWidget,2,0);
    mainLayout->addWidget(bottomTableWidget,4,0);
    mainLayout->addWidget(directoryView,4,1);


    //setLayout(mainLayout);
    //setMainWidget (mainSplitter);


    // delete these:
    QHBoxLayout * lists_layout = new QHBoxLayout;
    mainLayout->addLayout(lists_layout,0,1);
    lists_layout->addWidget(listNamesWidget);
    lists_layout->addWidget(listWidget);


    QWidget * middle_right_widget = new QWidget(this);
    QGridLayout * small_grid_layout = new QGridLayout;
    middle_right_widget->setLayout(small_grid_layout);
    rightSplitter->addWidget(middle_right_widget);
    //delete this one line:
    mainLayout->addLayout(small_grid_layout,2,1);
    m_proposed_new_title_label = new QLabel("Proposed new title") ;
    m_proposed_new_title_widget = new QLineEdit();
    m_new_list_name_widget = new QLineEdit("(enter name of new list here)");
    m_change_filename_button = new QPushButton("Change file name to (^K):");
    m_change_root_directory = new QPushButton("Change root directory");
    m_create_new_list_button = new QPushButton("Create new list (^N):");
    m_save_biblio_file_button = new QPushButton("Save biblio file (^S)");
    m_add_to_list_button = new QPushButton("Add entry to selected list (^U)");
    m_link_two_entries = new QPushButton("Link top and bottom entries (^Z)");
    m_delete_selected_files_button = new QPushButton("Delete selected files");

    small_grid_layout->addWidget(m_create_new_list_button,0,0);
    small_grid_layout->addWidget(m_new_list_name_widget,0,1);
    //small_grid_layout->addWidget(m_proposed_new_title_label,1,0);
    small_grid_layout->addWidget(m_change_filename_button,1,0);
    small_grid_layout->addWidget(m_proposed_new_title_widget,1,1);

    small_grid_layout->addWidget(m_change_root_directory,2,0);
    small_grid_layout->addWidget(m_save_biblio_file_button,3,0);
    small_grid_layout->addWidget(m_add_to_list_button,4,0);
    small_grid_layout->addWidget(m_link_two_entries,5,0);
    small_grid_layout->addWidget(m_delete_selected_files_button,6,0);
    m_current_list = nullptr;

    rightSplitter->addWidget(directoryView);



    connect(topTableView,SIGNAL(clicked(QModelIndex)) ,
            this , SLOT(on_top_table_view_clicked(QModelIndex)));
    connect(topTableView,SIGNAL(doubleClicked(QModelIndex)) ,
            this , SLOT(on_top_table_view_doubleClicked(QModelIndex)));
    connect(middleTableWidget,SIGNAL(cellChanged(int,int)) ,
            this , SLOT(on_middle_widget_item_changed(int,int)));
    connect(bottomTableWidget,SIGNAL(clicked(QModelIndex)) ,
            this , SLOT(on_bottom_table_widget_clicked(QModelIndex)));
    connect(bottomTableWidget,SIGNAL(cellDoubleClicked(int,int)) ,
            this , SLOT(on_bottom_table_widget_doubleClicked(int,int)));
    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)) ,
            this , SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));
    connect(m_change_root_directory,SIGNAL(clicked()),
                this,SLOT(set_new_root_folder()));
    connect(m_change_filename_button,SIGNAL(clicked()),
                this,SLOT(set_new_filename()));
    connect(listNamesWidget,SIGNAL(itemClicked(QListWidgetItem*)),
                this,SLOT(select_new_list(QListWidgetItem*)));
    connect(m_create_new_list_button,SIGNAL(clicked()),
                this,SLOT(new_list()));
    connect(m_save_biblio_file_button,SIGNAL(clicked()),
                this,SLOT(write_bibliography()));
    connect(m_add_to_list_button,SIGNAL(clicked()),
                this,SLOT(add_entry_to_list()));
    connect(m_delete_selected_files_button,SIGNAL(clicked()),
                this,SLOT(delete_selected_files()));

    m_keyCtrlA = new QShortcut(this);
    m_keyCtrlA->setKey(Qt::CTRL + Qt::Key_A);
    connect(m_keyCtrlA, SIGNAL(activated()), this, SLOT(show_files_with_same_size()));

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
    connect(m_keyCtrlK, SIGNAL(activated()), this, SLOT(set_new_filename()));

    /*
    m_keyCtrlL = new QShortcut(this);
    m_keyCtrlL->setKey(Qt::CTRL + Qt::Key_L);
    connect(m_keyCtrlL, SIGNAL(activated()), this, SLOT(write_lists_to_json()));
    */

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
}
QString from_fullfile_to_filestem(QString fullfilename){
    return fullfilename.mid(fullfilename.lastIndexOf("/") + 1);
}
void Widget::keyPressEvent(QKeyEvent *event)
{

}

/*                WIDGETS             */
Widget::~Widget()
{
   foreach(Entry * entry, m_data_by_key){
      delete entry;
   }
}

/*          LISTS           */
void Widget::select_new_list(QListWidgetItem * item){
    save_current_list();
    List* list;
    /* item is from List-of-lists listwidget */
    list = m_Lists_map[item->text()];
    display_list(list);
    m_current_list = list;
}


void Widget::save_current_list(){
    if (!m_current_list) {return;}
    qDebug() << 136 << "I will try to save the old list";
    for (int row = 0; row < listWidget->count(); row++){
        QString string = listWidget->item(row)->text();
        qDebug() << 127 << string << "Saving current list. Not implemented, though !!!";
    }
}
void Widget::display_list(List* list){
    listWidget->clear();
    foreach (Entry * entry, *list->get_entries()){
        listWidget->addItem(entry->get_author());         
    }
}
void Widget::new_list( ){
    QString list_name = m_new_list_name_widget->text();
    List* list = new List(list_name);
    if (m_Lists_map.contains(list_name)){
        qDebug() << "Attempt to create a new list when it already exists.";
    }
    m_Lists.append(list);
    m_Lists_map[list_name] = list;
    listNamesWidget->addItem(list_name);
}
void Widget::new_list(QString list_name = QString() ){
    List* list = new List(list_name);
    if (m_Lists_map.contains(list_name)){
        qDebug() << "Attempt to create a new list when it already exists.";
    }
    m_Lists.append(list);
    m_Lists_map[list_name] = list;
    listNamesWidget->addItem(list_name);

}
void Widget::add_entry_to_list( ){
    // This is from middle widget
    List* list = m_current_list;
    if (!list) {return;}
    list->add_entry(m_entry_in_middle_table);
    qDebug() << 294 << "author" << m_entry_in_middle_table->get_author();
    display_list(list);
    add_entry_to_bibmodel(m_entry_in_middle_table);
}
void Widget::add_entry_to_list(List* list, Entry* entry ){
    // This is from middle widget
    if (!list) {return;}
    list->add_entry(entry);
    display_list(list);
    //add_entry_to_bibmodel(m_entry_in_middle_table);
}

void Widget::change_location_of_listsInfo(){
    QString filename= QFileDialog::getSaveFileName(this, "Choose File", QDir::home().absolutePath(), "JSON files (*.json)");
    if(filename.isEmpty())
            return;
    if (! filename.endsWith(".json")){
        filename += ".json";
    }
    QFile fileIn(filename);
    int slash_loc = filename.lastIndexOf("/") + 1;
    QString foldername = filename.left(slash_loc);    
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
            return;
    m_settings.setValue("listsfilename", filename);
    m_settings.setValue("listsfoldername", foldername);
    qDebug() << 194 << filename << foldername;
}

/*                        READING FILES                      */



int function2 (QString line, Entry * entry, int n){ //file_info * this_file_info){
    if (line.endsWith("]")){line.chop(1);}
    if (line.startsWith("[ ")) { line.remove(0,2);}
    QStringList pieces = line.split(".pdf,");
    QString filenamestem = pieces[0];
    QString fullfilename = pieces[1];
    QString filesize = pieces[2];
    int i_filesize = filesize.toUInt();
    if (fullfilename.startsWith(" ../../")){
        fullfilename = fullfilename.mid(7);
    }
    filenamestem += ".pdf";
    fullfilename += ".pdf";
    QString key = "possible_file::" + QString::number(n);
    entry->set_info(key, fullfilename );
    //qDebug() << 276 << key  <<  fullfilename;
    return i_filesize;
}
int function3 (QString line, Entry * entry){ //file_info * this_file_info){
    if (line.endsWith("]")){line.chop(1);}
    if (line.startsWith("[ ")) { line.remove(0,2);}
    QStringList pieces = line.split(".pdf,");
    QString filenamestem = pieces[0];
    QString fullfilename = pieces[1];
    QString filesize = pieces[2];
    int i_filesize = filesize.toUInt();
    return i_filesize;
}
void Entry::read_json(QJsonObject & json){
    foreach (QJsonValue key_json, json.keys()){
        QString key = key_json.toString();
        qDebug() << 349 << key;
        if (key == "size"){
            size = json[key].toInt();
        }else{
            info[key] = json[key].toString() ;
        }
        qDebug() << 354 << key << info[key] << get_size();
    }
}
void Widget::read_JSON_file_new(){
    QJsonObject json_bibliography;
    QString filename= QFileDialog::getOpenFileName(this, "Choose File", m_json_folder, "JSON files (*.json)");
    if(filename.isEmpty())
        return;
    QFile fileIn(filename);
    int slash_loc = filename.lastIndexOf("/") + 1;
    QString foldername = filename.left(slash_loc);
    m_json_folder= foldername;
    if (!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QByteArray bytearray = fileIn.readAll();
    QJsonParseError parseError;
    QJsonDocument json_doc =  QJsonDocument::fromJson(bytearray, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    } else {
            json_doc.toJson(QJsonDocument::Compact);
    }

    m_settings.setValue("jsonfilename", filename);
    m_settings.setValue("jsonfoldername", foldername);
    fileIn.close();

    bool NewFormatFlag = true;
    if (NewFormatFlag){
        if(!json_doc.isArray()){
                qDebug() << "JSON doc root is not an array.";
        }
        QJsonObject  json_settings;
        json_settings = json_doc[0].toObject();
        // not doing anything currently.


        json_bibliography = json_doc[2].toObject();
    } else {
        // Old format::
        if(!json_doc.isObject()){
                qDebug() << "JSON doc root is not an object.";
        }
        json_bibliography = json_doc.object();
    }


    if(json_bibliography.isEmpty()){
        qDebug() << "The array is empty";
    }
    //int upper_table_row_number = 0;
    foreach (QString item_key, json_bibliography.keys()){     
        if(! json_bibliography[item_key].isObject()) {continue;}
        QJsonObject j_this_entry =  json_bibliography[item_key].toObject();
        if (j_this_entry.count() > 0){
                Entry* entry = new Entry();
                foreach (QString entry_internal_key, j_this_entry.keys()){
                    if (entry_internal_key == "size"){
                        entry->set_size(j_this_entry["size"].toInt());
                    } else {
                        QString value = j_this_entry[entry_internal_key].toString();
                        entry->set_info(entry_internal_key, value);
                    }
                }
                promote_file_from_preferred_location(entry);
                biblioModel->add_entry(entry);


                add_entry_to_bibmodel(entry);
                //upper_table_row_number++;
        }
    }

    QSortFilterProxyModel * proxyModel = new QSortFilterProxyModel () ;
    proxyModel->setSourceModel( biblioModel );
    topTableView->setModel( proxyModel );

    topTableView->setColumnWidth(0,400);
    topTableView->setColumnWidth(1,400);
    topTableView->setColumnWidth(2,400);
    topTableView->setColumnWidth(3,100);
    // What follows has to be done *after* the entries have been loaded: this is the List information.
   if (NewFormatFlag){
       QJsonArray json_lists_array;
       json_lists_array = json_doc[1].toArray();

       QJsonObject json_list_object ;
       List* list;
       for (int n = 0; n < json_lists_array.size(); n++){
           json_list_object = json_lists_array[n].toObject();
           QString list_name = json_list_object["list_name"].toString();
           if (m_Lists_map.contains(list_name)){
               list = m_Lists_map[list_name];
           } else{
               List * list = new List (json_list_object["list_name"].toString());
           }
           QJsonArray json_entries;
           json_entries = json_list_object["entries"].toArray();
           //qDebug() << 411 << json_entries.count();
           Entry * entry;
           for (int n = 0; n < json_entries.size(); n++){
               {
                   QJsonObject list_entry;
                   list_entry = json_entries[n].toObject();                   
                   QList<Entry*> entries = get_entries_by_size(list_entry["size"].toInt());
                   entry = entries.first();
                   add_entry_to_list(list, entry);
               }
            }
       }
    }
   link_top_and_bottom_entries_from_size();
   link_top_and_bottom_entries_from_filename();
}
QList<Entry*>  Widget::get_entries_by_size(int n){
    QList<Entry*>  list = m_data_by_size.values(n);
    return list;
}

void Widget::read_JSON_file_old()
{
}

/*           WRITE TO FILE              */
void write_list_to_json(List* list, QJsonArray & json_array ){
    QJsonObject this_list;
    this_list["list_name"] = list->get_name();
    QJsonArray this_list_entries;
    for (int n=0; n < list->get_count_of_entries(); n++){
        QJsonObject this_entry;
        Entry* entry = list->get_entry(n);
        this_entry["key"]= entry->get_key();
        this_entry["title"]= entry->get_title();
        this_entry["author"]= entry->get_author();
        this_entry["year"]= entry->get_year();
        this_entry["size"]= entry->get_size();
        this_list_entries.append(this_entry);
    }
    this_list["entries"] = this_list_entries;
    json_array.append(this_list);
}


void Widget::write_lists_to_json(QJsonArray & json_array){
    foreach (List* list, m_Lists){
        write_list_to_json (list,json_array);
    }
}
void Widget::write_bibliography(){
    write_bibliography_to_json();
    write_bibliography_to_bibtex();
}

void Widget::write_bibliography_to_json( ){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyy_MM_dd_hh:mm:ss");
    qDebug() << 487 << formattedTime;
    QJsonArray json_top;
    QJsonObject json_settings;
    json_settings["m_root_folder"] = m_root_folder;
    json_settings["m_json_folder"] = m_json_folder;
    json_top.append(json_settings);

    QJsonArray json_array_for_lists;
    write_lists_to_json(json_array_for_lists);
    json_top.append(json_array_for_lists);

    QJsonObject json_object_for_biblio;
    foreach (QString doc_key, m_data_by_key.keys()){
        write_bibentry_to_json(m_data_by_key[doc_key], json_object_for_biblio ); //TODO  HERE IS WHERE I stopped. have to iterate over the right key.
    }
    json_top.append(json_object_for_biblio);

    QStringList filenames;
    filenames << "bibliography.json" << formattedTime + "bibliography.json";
    foreach (QString this_filename, filenames){
        QString fullfilename(m_json_folder + this_filename );
        QFile saveFile(fullfilename);
        if (!saveFile.open(QIODevice::WriteOnly)) {
             qWarning("Couldn't open save file.");
             return;
        }
        qDebug() << 610 << fullfilename;
        QJsonDocument doc(json_top);
        saveFile.write(doc.toJson());
    }
}
void Widget::write_bibliography_to_bibtex(){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyy_MM_dd_hh:mm:ss");
    QString filename = m_json_folder + "this_bibliography.bibtex";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
         qWarning("Couldn't open save file.");
         return;
    }
    QTextStream stream(&file);
    foreach (QString doc_key, m_data_by_key.keys()){
        m_data_by_key[doc_key] -> write_bibentry_to_bibtex(  stream , m_bibliography_labels); //TODO  HERE IS WHERE I stopped. have to iterate over the right key.
    }
    qDebug() << 770 << filename;
    file.close();
}

void Entry::write_bibentry_to_bibtex( QTextStream & stream, QStringList & bibliography_labels){
    stream << QString("@");
    QString mytype, author, title, year, key;
    if (info["type"].length() == 0) {
        mytype = "article";
    } else {
        mytype = info["type"];
    }
    stream << mytype << "{" << info["key"] << "{ \n";
    if (info["author"].length() == 0) {
        author = "author_unknown";
    } else {
        author = get_author();
    }
    stream << "   " << "author = \"" << author << "\",\n" ;

    if (info["title"].length() == 0) {
        title = "no title";
    } else {
        title = get_title();
    }
    stream << "   " << "title  = \"" << title << "\",\n" ;

    if (info["year"].length() == 0) {
        year = "no year";
    } else {
        year = get_year();
    }
    stream << "   " << "year   = \"" << year << "\",\n" ;

    if (info["key"].length() == 0) {
        key = "no key";
    } else {
        key = get_key();
    }
    stream << "   " << "key    = \"" << key << "\",\n" ;

    if (get_size() > 0) {
        stream << "   " << "size    = \"" << get_size() << "\"" ;
    }

    QStringList first_labels;
    first_labels << "type" << "author" << "title" << "year" << "key" << "size";

    foreach (QString label, bibliography_labels){
        if (first_labels.contains(label)){continue;}
        if (get_info(label).length() > 0){
            stream << ",\n";
            stream << "   "<< label <<  get_info(label);
        }
    }

    int fileno = 0;
/*
    QString file = "possible_file::" + QString::number(fileno);
    while(entry->contains_info(file)){
        this_json_entry[file] = entry->get_info(file);
        qDebug() << 589 << entry->get_info(file);
        fileno++;
        file = "possible_file::" + QString::number(fileno);
    }
    */

    stream << "\n}\n";

}

void Widget::write_bibentry_to_json(Entry * entry, QJsonObject & total_json_object)
{    QString key;
    QJsonObject this_json_entry;
    if (entry->get_size() > 0){
        this_json_entry["size"] = entry->get_size();
    }
    foreach (QString label, m_bibliography_labels){
        if (entry->get_info(label).length() > 0){
            this_json_entry[label] = entry->get_info(label);
        }
        if (label=="key"){
            key = entry->get_info("key");
        }
    }
    int fileno = 0;
    QString file = "possible_file::" + QString::number(fileno);
    while(entry->contains_info(file)){
        this_json_entry[file] = entry->get_info(file);
        qDebug() << 589 << entry->get_info(file);
        fileno++;
        file = "possible_file::" + QString::number(fileno);
    }
    total_json_object[key] = this_json_entry;
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
QString invert_first_author(QString authors){
    QString surname, first_author, result;
    if (authors.length()==0) {return "author";}
    QStringList author_list = authors.split("and");
    first_author = author_list[0].trimmed();
    if (! first_author.contains(",")) {
       QStringList name_list = first_author.split(" ");
       QString surname = name_list.takeLast() + ",";
       name_list.prepend(surname);
       first_author = name_list.join(" ");
       author_list.replace(0, first_author);
    }
     result = author_list.join (" and ");
    return result;
}

/*                 TOP VIEW                     */


// THis is no longer used -- the Qt internal functions are used model/view
void Widget::add_entry_to_top_view (Entry* entry){

}

void Widget::on_top_table_view_clicked(const QModelIndex &index){
    qDebug() << index << index.row() << index.column() << 851;
    put_bibitem_info_on_middle_table_widget(index);
}

void Widget::on_top_table_view_doubleClicked(const QModelIndex &index){
    //qDebug() << 712;
    int row = index.row();
    QString key  = bibModel->item(row,3)->text();
    //qDebug() << 715 << key;
    Entry * entry = m_data_by_key[key];
    m_entry_in_top_table = entry;
    QString filename = entry->get_info("filenamefull");
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}






/*              BOTTOM VIEW                 */
// This should really be a function of the lower widget, if I subclass it.
// Currently this is only used by a function no longer used, one that reads the old format. It should be removed.
void::Widget::mark_file_as_indexed(QString filename){
    if (! filename.endsWith(".pdf")){
        filename += ".pdf";
    }
    for (int rowno = 0; rowno < bottomTableWidget->rowCount(); rowno++){
        QString this_file_name = bottomTableWidget->item(rowno,0)->text() +
                bottomTableWidget->item(rowno,1)->text();
        if (filename == this_file_name){
            QTableWidgetItem * item = bottomTableWidget->item(rowno,2);
            item->setForeground(QBrush(QColor(0,255, 255)));
        }
    }
}

void Widget::on_bottom_table_widget_clicked(const QModelIndex & index ){
    int row = index.row();
    put_file_info_on_middle_table_widget(row);
}

void Widget::on_bottom_table_widget_doubleClicked(int row, int column){
    QTableWidgetItem  * itemfolder =  bottomTableWidget->item(row,2);
    QString foldername = itemfolder->data(Qt::DisplayRole).toString();
    QTableWidgetItem* itemfilename = bottomTableWidget->item(row,1);
    QString filename = itemfilename->data(Qt::DisplayRole).toString();
    QString completename = foldername + "/" +  filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(completename));
    Entry* entry = m_files_onboard_by_filenamefull[completename];
    m_entry_in_bottom_table = entry;
}
void Entry::remove_bottom_view_links(){
    m_bottom_view_size_item = nullptr;
    m_bottom_view_filename_item = nullptr;
    }
void Widget::search_folders_for_pdf()
{
    // For each entry in the top model, remove the link to items in the bottom widget:

    foreach (QString key, m_data_by_key.keys()){
        qDebug() << 836 << key;
        m_data_by_key[key]->remove_bottom_view_links();
    }

    bottomTableWidget->clear();
    bottomTableWidget->setColumnWidth(0,40);
    bottomTableWidget->setColumnWidth(1,500);
    bottomTableWidget->setColumnWidth(2,500);
    bottomTableWidget->setColumnWidth(3,100);
    bottomTableWidget->setColumnWidth(4,100);
    bottomTableWidget->setColumnWidth(5,100);
    m_files_onboard_by_filenamefull.clear();
    m_files_onboard_by_filenamestem.clear();
    m_files_onboard_by_size.clear();
    bottomTableWidget->clear();
    QString targetStr = ".pdf"; // What we search for
    QString filenameStem, filenameFull, folder;
    int size;
    QFileInfoList hitList; // Container for matches
    QDirIterator it(m_root_folder, QDirIterator::Subdirectories);
    while (it.hasNext()) {
            QString filename = it.next();
            QFileInfo file(filename);           
            if (file.isDir()) { // Check if it's a dir
                continue;
            }
            if (file.fileName().endsWith(targetStr, Qt::CaseInsensitive)) {
                hitList.append(file);
            }
        }
    bottomTableWidget->setRowCount(hitList.count());
    bottomTableWidget->setColumnCount(6);
    int rowno = 0;
    int count = 0;
    QTableWidgetItem * item0, *item1,*item2,*item3, *item4, *item5;
    foreach (QFileInfo hit, hitList) {
        filenameStem = hit.fileName();
        folder = hit.dir().absolutePath();
        size = hit.size();
        if (size == 0){
            continue;
        }
        filenameFull = folder + "/" + filenameStem;
        Entry * entry = new Entry();
        entry->set_folder(folder);
        entry->set_filenameStem(filenameStem);
        entry->set_filenameFull(filenameFull);
        entry->set_info("date", hit.lastModified().date().toString("yyyy MM dd"));
        entry->set_info("lastread", hit.lastRead().date().toString("yyyy MM dd"));
        //qDebug() << 1046 << hit.birthTime().toString("yyyy MM dd") << hit.absoluteFilePath() << hit.lastModified().date().toString();
        //qDebug() << 880 << entry->get_filenamefull();
        entry->set_size(size);
        if (entry->get_size() == 0){
            qDebug() << 882 << "size zero";
            continue;
        }
        m_files_onboard_by_filenamefull[filenameFull] = entry;
        m_files_onboard_by_filenamestem.insert(filenameStem, entry);
        if (m_files_onboard_by_size.contains(size)){
            count++;
        }
        m_files_onboard_by_size.insert(size, entry);
        item0 = new QTableWidgetItem();
        item0->setCheckState(Qt::Unchecked);
        item1 = new QTableWidgetItem(filenameStem);
        item2 = new QTableWidgetItem(folder);
        item3 = new QTableWidgetItem();
        item3->setData(Qt::DisplayRole, size);
        item4 = new QTableWidgetItem();
        item4->setData(Qt::DisplayRole, entry->get_info("date"));
        item5 = new QTableWidgetItem();
        item5->setData(Qt::DisplayRole, entry->get_info("lastread"));
        entry->set_bottom_view_filename_item(item1);
        entry->set_bottom_view_size_item(item3);
        bottomTableWidget->setItem(rowno,0,item0);
        bottomTableWidget->setItem(rowno,1,item1);
        bottomTableWidget->setItem(rowno,2,item2);
        bottomTableWidget->setItem(rowno,3,item3);
        bottomTableWidget->setItem(rowno,4,item4);
        bottomTableWidget->setItem(rowno, 5, item5);
        rowno++;
    }
    bottomTableWidget->setSortingEnabled(true);
    link_top_and_bottom_entries_from_size(); // TODO: clear out any previous linkings before doing this;
    link_top_and_bottom_entries_from_filename(); // TODO: clear out any previous linkings before doing this;
    //show_files_with_same_size();
}
void Widget::delete_selected_files(){
    for(int row = 0; row < bottomTableWidget->rowCount(); row++){
        if (bottomTableWidget->item(row,0)->checkState()==Qt::Checked){
            QString filename = bottomTableWidget->item(row,2)->text() + "/" + bottomTableWidget->item(row,1)->text();
            QFile file(filename);
            file.remove();
        }
    }
}
void Widget::show_files_with_same_size(){
    bottomTableWidget->clear();    
    QList<int> sizes = m_files_onboard_by_size.uniqueKeys();
    int row = 0;
    int start_row = 0;
    int stop_row = 0;
    QList<QString> filenames;
    QTableWidgetItem* item0, *item1, *item2, *item3, *item4, *item5;
    foreach (int size, sizes){
        if (m_files_onboard_by_size.count(size) > 1){
            QTableWidgetItem * item1, * item2, * item3, *item0;
            start_row = row;
            foreach(Entry* entry, m_files_onboard_by_size.values(size)){
                item0 = new QTableWidgetItem();
                item0->setCheckState(Qt::Unchecked);
                item1 = new QTableWidgetItem(entry->get_filenamestem());
                item2 = new QTableWidgetItem(entry->get_folder());
                item3 = new QTableWidgetItem();
                item3->setData(Qt::DisplayRole, size);
                item4 = new QTableWidgetItem();
                item4->setData(Qt::DisplayRole, entry->get_info("date"));
                item5 = new QTableWidgetItem();
                item5->setData(Qt::DisplayRole, entry->get_info("lastread"));
                entry->set_bottom_view_filename_item(item1);
                entry->set_bottom_view_size_item(item3);
                bottomTableWidget->setItem(row,0,item0);
                bottomTableWidget->setItem(row,1,item1);
                bottomTableWidget->setItem(row,2,item2);
                bottomTableWidget->setItem(row,3,item3);
                bottomTableWidget->setItem(row,4,item4);
                bottomTableWidget->setItem(row, 5, item5);
                stop_row = row;
                row++;
            }

        }
    }
    bottomTableWidget->setRowCount(row);
    bottomTableWidget->setColumnWidth(0,40);
    bottomTableWidget->setColumnWidth(1,500);
    bottomTableWidget->setColumnWidth(2,500);
    bottomTableWidget->setColumnWidth(3,100);
    bottomTableWidget->setColumnWidth(4,100);
    bottomTableWidget->setColumnWidth(5,100);
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
                entry_top->set_size_item(this_size);
                foreach(Entry* entry_bottom, m_files_onboard_by_size.values(this_size) ){                    
                    entry_top->add_to_on_board_entries(entry_bottom);
                    entry_bottom->add_to_bib_entries(entry_top);
                    entry_bottom->color_bottom_view_item_for_size();
                }
            }
        }
    }
}
// Don't use this next function??
void Entry::set_size_item(int size){
    QStandardItem * item;
    if (m_top_view_size_item) {
        m_top_view_size_item->setText(QString::number(size));
    }
    if (m_bottom_view_size_item) {
       m_bottom_view_size_item->setText(QString::number(size));
    }
}
void Entry::color_bottom_view_item_for_size(){
    QTableWidgetItem* item_bottom = m_bottom_view_size_item;
    item_bottom->setBackground(QBrush(QColor(0,255, 255)));
}
void Entry::color_bottom_view_item_for_filename(){
    QTableWidgetItem* item_bottom = m_bottom_view_filename_item;
    item_bottom->setBackground(QBrush(QColor(0,255, 255)));
}
void Entry::color_top_view_item_for_size(){
    QStandardItem* item_top = m_top_view_size_item;
    item_top->setBackground(QBrush(QColor(0,255, 255)));
}
void Entry::color_top_view_item_for_filename(){
    QStandardItem* item_top = m_top_view_filename_item;
    item_top->setBackground(QBrush(QColor(0,255, 255)));
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
                //entry_top->color_top_view_item_for_filename();
                entry_bottom->color_bottom_view_item_for_filename();
                entry_top->add_to_on_board_entries(entry_bottom);
                entry_bottom->add_to_bib_entries(entry_top);
            }
        }
    }
}
void Widget::link_top_and_bottom_entries(){
   int column_for_size = 5;
   QStandardItem * item = bibModel->item(m_selected_row_in_top_table, column_for_size);
   item->setText(QString::number(m_entry_in_bottom_table->get_size()));
   m_entry_in_top_table->set_size(m_entry_in_bottom_table->get_size());
   m_entry_in_top_table->set_filenameStem(m_entry_in_bottom_table->get_filenamestem());
   m_entry_in_top_table->set_folder(m_entry_in_bottom_table->get_folder());
   m_entry_in_top_table->set_filenameFull(m_entry_in_bottom_table->get_filenamefull());
}

/*              MIDDLE           VIEW           */

void Widget::set_new_filename(){
    QString new_name = m_proposed_new_title_widget->text();
    if (new_name.length() == 0) {return;}
    Entry* entry = m_entry_in_middle_table;
    QString author = middleTableWidget->item(0,1)->text();
    QString title =  middleTableWidget->item(1,1)->text();
    QString year =  middleTableWidget->item(2,1)->text();
    QString old_filestem = middleTableWidget->item(4,1)->text();
    qDebug() << 1000 << old_filestem;
    QString full_old_name = entry->get_filenamefull();
    QString full_new_name = entry->get_folder() + "/" + new_name;
    QFileInfo   fileinfo (full_old_name);
    QFile file(full_old_name);
    if (fileinfo.exists()){
        bool value = file.rename(full_new_name);
        if (value) {qDebug() << "Success! changed name";} else{qDebug() << "Failure to change.";}
    }else{
        qDebug() << 889 << "Hmm, couldnt find that file.";
    }
    qDebug() << 823 << full_old_name << full_new_name<< "\n" ;
    entry->set_filenameStem(new_name);
    entry->set_filenameFull(full_new_name);
    entry->set_author(author);
    entry->set_title(title);
    entry->set_year(year);
    update_data_by_fullfilename(full_old_name, full_new_name,entry);
    update_files_onboard_by_fullfilename(full_old_name, full_new_name, entry);
    update_files_onboard_by_filenamestem(old_filestem, new_name, entry);
    //entry->get_bottom_view_filename_item()->setText(new_name);
    entry->set_filename_item_bottom(new_name);
    m_files_onboard_by_filenamestem.remove(old_filestem);
    m_files_onboard_by_filenamestem.insert(new_name, entry);
    m_files_onboard_by_filenamefull[full_new_name] = entry;
    qDebug() << 899 << entry->display();
}
void Entry::set_filename_item_bottom(QString filename){
    m_bottom_view_filename_item->setText(filename);
}
void Widget::update_data_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry){
    m_data_by_fullfilename.remove(full_old_name);
    m_data_by_fullfilename[full_new_name] = entry;
}
void Widget::update_files_onboard_by_fullfilename(QString full_old_name, QString full_new_name, Entry* entry){
    m_files_onboard_by_filenamefull.remove(full_old_name);
    m_files_onboard_by_filenamefull[full_new_name] = entry;
}
void Widget::update_files_onboard_by_filenamestem(QString oldfilename, QString newfilename, Entry* entry){
   // m_files_onboard_by_filenamestem.
}

void Widget::display_entry_on_middle_table(){
    Entry * entry = m_entry_in_middle_table;
    middleTableWidget->setRowCount(m_bibliography_labels.length());
    middleTableWidget->setColumnCount(4);
    middleTableWidget->clear();
    middleTableWidget->setColumnWidth(1,500);
    QTableWidgetItem * item;
    QStringList labels(m_bibliography_labels);
    int row_number (0);
    foreach (QString label, m_bibliography_short_labels){
        if (label == "size"){
            item = new QTableWidgetItem(label);
            middleTableWidget->setItem(row_number,0,item);
            item = new QTableWidgetItem(QString::number(entry->get_size()));
            middleTableWidget->setItem(row_number,1,item);
            row_number++;
            labels.removeAll(label);
            continue;
        }
        QString value = entry->get_info(label);
        item = new QTableWidgetItem(label);
        qDebug() << 923 << label << value;
        middleTableWidget->setItem(row_number,0,item);
        item = new QTableWidgetItem(value);
        middleTableWidget->setItem(row_number,1,item);
        row_number++;
        labels.removeAll(label);
    }
    labels.sort();
    foreach (QString label, labels){
       item = new QTableWidgetItem(label);
       middleTableWidget->setItem(row_number,0,item);
       row_number++;
    }
}



void Widget::put_bibitem_info_on_middle_table_widget(const QModelIndex & index){
    QTableWidgetItem * item;
    QModelIndex index2;
    int row = index.row();
    int col = 3;
    index2 = biblioModel->index(row,col);
    QString key = biblioModel->data(index2).toString();
    //QString key = biblioModel->item(row,3)->text();
    Entry* entry = m_data_by_key[key];
    m_entry_in_top_table  = entry;
    m_selected_row_in_top_table = row;
    m_entry_in_middle_table = entry;
    display_entry_on_middle_table();
    m_proposed_new_title_widget->clear();
}
void Widget::put_file_info_on_middle_table_widget(int bottom_widget_row){
    QString filename(bottomTableWidget->item(bottom_widget_row,1)->text());
    QString foldername(bottomTableWidget->item(bottom_widget_row,2)->text());
    QString fullname = foldername + "/" + filename;
    qDebug() << 1067 << fullname;
    Entry* entry = m_files_onboard_by_filenamefull[fullname];
    m_entry_in_bottom_table = entry;
    m_entry_in_middle_table = entry;
    display_entry_on_middle_table();
    m_proposed_new_title_widget->clear();
}
void Widget::on_middle_widget_item_changed(int row, int column ){
    if (  ( bottomTableWidget->hasFocus() || middleTableWidget->hasFocus())  &&
          (row == 3 || row == 1 || row == 2)  )
    {
        generate_new_title();
    }
}
/*              BIB MODEL               */
void Widget::add_entry_to_bibmodel(Entry* entry){

    add_entry_to_bibmodel_by_key(entry);
    add_entry_to_bibmodel_by_filenamestem(entry);
    add_entry_to_bibmodel_by_fullfilename(entry);
    add_entry_to_bibmodel_by_size(entry);
    //add_entry_to_top_view(entry); // ???
}
void Widget::add_entry_to_bibmodel_by_size(Entry * entry ){

    int this_size = entry->get_size();
    m_data_by_size.insert(this_size, entry);
}
void Widget::add_entry_to_bibmodel_by_fullfilename(Entry* entry){
    m_data_by_fullfilename[entry->get_filenamefull()] = entry;
}
void Widget::add_entry_to_bibmodel_by_filenamestem(Entry* entry){
    m_data_by_filenamestem.insert(entry->get_filenamestem(), entry);
}
void Widget::add_entry_to_bibmodel_by_key(Entry* entry){
    QString key;
    if (entry->get_key().length() > 0 && m_data_by_key.contains(entry->get_key())){
        m_filename_collisions.insert(entry->get_key(), entry);
        qDebug() << 594 << "collision by key" << entry->get_key();
        //QMessageBox box;
        //box.setText("Key collision " + entry->get_key());
        //box.exec();
    } else{
        if (entry->get_key().length() > 0){            
            m_data_by_key[entry->get_key()] = entry;
            //qDebug() << 1098 << entry->get_key();
        } else {
            key = entry->get_filenamestem();
            entry->set_key(key);
            m_data_by_key[key] = entry;
            //qDebug() << 1103 << key;
        }
    }
}
QString find_surname(QString authors){
    QString author;
    QStringList names;
    QStringList suffixes;
    suffixes  << "Jr." << "Sr.";
    if (authors.length() == 0) {return "author not specified";}
    if (authors.contains(" and ")){
        QStringList author_list = authors.split("and");
        author = author_list[0].trimmed();
    } else{
        author = authors;
    }
    if (author.contains(",")){
        return author.split(",")[0];
    }
    if (author.contains(" ")){
        names = author.split(" ");
        int ct = names.count();
        if (suffixes.contains(names[ct-1]))
            {return names[ct-2];}
        else{
            return names[ct-1];
        }
    } else{
        return author;
    }
}

void Widget::generate_new_title(){
    QString author, title, new_title, year, new_filename;
    if (middleTableWidget->item(0,1)) {
        author = middleTableWidget->item(0,1)->text();
        author = find_surname(author);
    }
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
    }
    if (year.length() > 0) { new_filename = year + "_";}
    if (author.length() > 0) { new_filename += author; }
    new_filename +=  new_title;
    new_filename += ".pdf";
    m_proposed_new_title_widget->setText(new_filename);
}

void Widget::on_listWidget_doubleClicked(QListWidgetItem* item){
    int row = listWidget->row(item);
    qDebug() << 724 << row;
    Entry * entry = m_current_list->get_entry(row);
    QString filename = entry->get_filenamefull();
    qDebug() << 728 << filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}
QString Widget::test_key_for_uniqueness (QString key){
    while (m_data_by_key.contains(key)){
        key += "@";
        qDebug() << 761 << "Collision of new key "<< key;
    }
    return key;
}

List::List(QString this_name){
    name = this_name;
}
Entry::Entry(){
    size = 0;
    m_top_view_size_item = NULL;
    m_top_view_filename_item = NULL;
    m_bottom_view_size_item = NULL;
    m_bottom_view_filename_item = NULL;
    m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
}
Entry::~Entry(){

}
Entry::Entry(QString stem, QString folder, qint64 a_size)
{
    int this_size = static_cast<int>(a_size);
    info["folder"] = folder;
    info["filenamestem"] = folder;
    info["filenamefull"] = folder + "/" + stem;
    size = this_size;
    m_top_view_size_item = NULL;
    m_top_view_filename_item = NULL;
    m_bottom_view_size_item = NULL;
    m_bottom_view_filename_item = NULL;
    m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
}
Entry::Entry(QString stem, QString folder, int this_size)
{
  size = this_size;
  QString filenameFull = folder + "/" + stem;
  set_folder(folder);
  set_filenameStem(stem);
  set_filenameFull(filenameFull);
  m_top_view_size_item = NULL;
  m_top_view_filename_item = NULL;
  m_bottom_view_size_item = NULL;
  m_bottom_view_filename_item = NULL;
  m_creation_time = QDateTime(QDate::currentDate(), QTime::currentTime());
}
void Entry::set_info(QString this_key,QString this_value)
{   if (this_key == "size"){
        size = this_value.toInt();
    } else{
        info.insert(this_key, this_value);
    }
}
void Entry::set_filenameFull(QString s) {
    if (get_filenamefull().length() == 0){
        info["filenamefull"] = s;
    } else{
        m_multiple_filenamefulls.append(s);
    }
}
QString Entry::display(){
    QString result;
    result = "[size: " + QString::number(get_size()) + "]  ";
    foreach (QString key, info.keys()){
        result += "[" +  key + " : " + info[key] + "]  " ;
    }
    return result;
}
QString Entry::get_info(QString key){
    if (info.contains(key)) {
        return info[key];
    }
    else {
        return QString();
    }
}
