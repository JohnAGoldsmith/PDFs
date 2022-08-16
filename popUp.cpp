#include <QTableWidget>
#include <QShortcut>
#include <QHeaderView>
#include "widget.h"

//extern void display_entry_on_tablewidget(QTableWidget* table_widget, Entry* entry, QStringList & my_labels);
//bextern void display_entry_on_tablewidget(QTableView* table_widget, Entry* entry, QStringList & my_labels);

PopUp::PopUp(QWidget* parent ){resizeColumnToContents(0);


     QShortcut *  m_keyCtrlRightBracket; // ] hide popUp
     m_keyCtrlRightBracket = new QShortcut(this);
     m_keyCtrlRightBracket->setKey(Qt::CTRL  + Qt::Key_BracketRight);
     connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(hide()));
     //connect(this, SIGNAL(itemChanged(PopUp)), &Widget,  SLOT(on_any_itemChanged( QTableWidgetItem*)));
     setGeometry(0,0,450,1400);
     horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Widget::open_popUp(){
    if (myPopUp== nullptr){
        myPopUp = new PopUp(this);
        m_keyCtrlRightBracket = new QShortcut(this);
        m_keyCtrlRightBracket->setKey(Qt::CTRL  + Qt::Key_BracketRight);
        connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(close_popUp()));
    }
    //display_entry_on_tablewidget(myPopUp, m_entry_in_middle_table ,  m_bibliography_labels);
    //display_entry_on_tableview(myPopUp, m_entry_in_bottom_table);
    EntryModel * entryModel = new EntryModel(this);
    myPopUp->setModel(entryModel);
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

