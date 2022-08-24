#include <QTableWidget>
#include <QShortcut>
#include <QHeaderView>
#include <QMessageBox>
#include "widget.h"

//extern void display_entry_on_tablewidget(QTableWidget* table_widget, Entry* entry, QStringList & my_labels);
//bextern void display_entry_on_tablewidget(QTableView* table_widget, Entry* entry, QStringList & my_labels);

PopUp::PopUp(Entry* entry, Widget* parent){resizeColumnToContents(0);
     QShortcut *  m_keyCtrlRightBracket; // ] hide popUp
     m_keyCtrlRightBracket = new QShortcut(this);
     m_keyCtrlRightBracket->setKey(Qt::CTRL  + Qt::Key_BracketRight);
     connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(hide()));
     setGeometry(0,0,450,1400);
     horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     qDebug() << 16 << entry->get_filenamefull();
     m_entry = entry;
     m_entryModel = new EntryModel(m_entry, parent);
     setModel(m_entryModel);
}

void Widget::open_popUp(){
    if (myPopUp== nullptr){
        if (!m_entry_in_bottom_table){
            QMessageBox box;

            box.setText( "Select an item first.");
            box.exec();
            return;
        }
        myPopUp = new PopUp(m_entry_in_bottom_table, this );
        m_keyCtrlRightBracket = new QShortcut(this);
        m_keyCtrlRightBracket->setKey(Qt::CTRL  + Qt::Key_BracketRight);
        connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(close_popUp()));
    } else{
        myPopUp->setEntry(m_entry_in_bottom_table);
    }

    myPopUp->show();
    myPopUp->raise();
}
void Widget::hide_popUp(){
    if (myPopUp){
        myPopUp->hide();
    }
}
void Widget::close_popUp(){
    if (myPopUp){
        myPopUp->hide();
    }
}
void PopUp::hide(){
    hide();
}
void PopUp::setEntry(Entry * entry){
    m_entry = entry;
}
