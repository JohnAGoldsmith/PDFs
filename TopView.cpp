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
#include <EGL/egl.h>

class List;



void Widget::on_top_table_view_clicked(const QModelIndex&index){

    QModelIndex underlying_index = biblioModel->m_proxyModel->mapToSource(index);
    int model_row = underlying_index.row();
    qDebug() <<1035 <<  index << "index row"<<index.row() << "model row" <<model_row ;
    Entry * entry = biblioModel->get_entries().at(model_row);
    m_entry_in_top_table  = entry;

    int column_for_size(5);
    if (index.column() == column_for_size){
        if (!entry->selected_for_deletion()   ){
            qDebug() << 1059 << "this box is not checked";
            entry->set_selected_for_deletion(true);
            qDebug() << 1061 << "this box is now checked";
        } else {
            qDebug() << 1063 << "Box was checked";
            entry->set_selected_for_deletion(false);
             qDebug() << 1061 << "this box is now not checked";
        }
    }
    biblioModel->dataChanged(underlying_index, underlying_index);

    //put_bibitem_info_on_middle_table_widget(index);


    put_bibitem_info_on_middle_table_widget(entry);


}

void Widget::on_top_table_view_doubleClicked(const QModelIndex&index){
    QModelIndex underlying_index = biblioModel->m_proxyModel->mapToSource(index);
    int model_row = underlying_index.row();
 qDebug() <<63   <<  index << "index row"<<index.row() << "model row" <<model_row ;
    Entry * entry = biblioModel->get_entries().at(model_row);

    QString filename =  entry->get_filenamefull();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));

}
