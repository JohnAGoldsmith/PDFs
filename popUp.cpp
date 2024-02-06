#include <QTableWidget>
#include <QShortcut>
#include <QHeaderView>
#include <QMessageBox>
#include "widget.h"
#include "EntryModel.h"

//extern void display_entry_on_tablewidget(QTableWidget* table_widget, Entry* entry, QStringList & my_labels);
//bextern void display_entry_on_tablewidget(QTableView* table_widget, Entry* entry, QStringList & my_labels);

PopUp::PopUp(EntryModel * selected_entry_model){
                        resizeColumnToContents(0);
     QShortcut *        m_keyCtrlRightBracket; // ] hide popUp
                        m_keyCtrlRightBracket = new QShortcut(this);
                        m_keyCtrlRightBracket->setKey(Qt::CTRL  | Qt::Key_BracketRight);
                        connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(hide()));
                        setGeometry(0,0,450,1400);
                        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                        if (selected_entry_model){
                            setModel(selected_entry_model);
                        }

}

void Widget::open_popUp(){
    if (myPopUp== nullptr){
        
        if (! m_selected_file ) {
            QMessageBox box;

            box.setText( "Select an item first.");
            box.exec();
            return;
        }

        myPopUp = new PopUp(m_selected_entry_model);
        m_keyCtrlRightBracket = new QShortcut(this);
        m_keyCtrlRightBracket->setKey(Qt::CTRL  | Qt::Key_BracketRight);
        connect(m_keyCtrlRightBracket, SIGNAL(activated()), this, SLOT(close_popUp()));
    } else{
      //  myPopUp->setEntry(m_selected_onboard_entry);

    }

    myPopUp->show();
    myPopUp->raise();
}
void Widget::hide_popUp(){
    if (myPopUp){
        myPopUp->setVisible(false);
    }
}
void Widget::close_popUp(){
    if (myPopUp){
        myPopUp->setVisible(false);
    }
}
//void PopUp::hide(){
//    hide();
//}



void PopUp::setEntry(Entry * entry){
    m_entry = entry;
}

