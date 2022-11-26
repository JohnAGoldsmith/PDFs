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

//class List;



QString get_first_author(QString names){
    if (names.length()==0) {return "author";}
    if (! names.contains(" and ")) { return names;}
    return names.split(" and ")[0].trimmed();
}
QString find_surname(QString name){
    if (name.length() == 0) {return "author";}
    if (name.contains(",")){
        return name.split(",")[0];
    }
    QStringList names;
    QStringList suffixes;
    suffixes  << "Jr." << "Sr.";
    if (name.contains(" ")){
        names = name.split(" ", Qt::SkipEmptyParts);
        int ct = names.count();
        if (suffixes.contains(names[ct-1]))
            {return names[ct-2] + names[ct-1];}
        else{
            return names[ct-1];}
    } else{
        return name;
    }
}
QString prepose_surname_if_necessary(QString name){
    if (name.length() == 0) {return "author";}
    if (name.contains(",")) {return name;}
    QStringList names;
    QStringList suffixes;
    suffixes  << "Jr." << "Sr.";
    if (name.contains(" ")){
        names = name.split(" ", Qt::SkipEmptyParts);
        int ct = names.count();
        if (suffixes.contains(names[ct-1])){
            names.prepend(names.takeLast() + ",");
            names.prepend(names.takeLast());
        }
        else{
            names.prepend(names.takeLast() + ",");
        }
        return names.join(" ");
    } else{
        return QString("author");
    }
}
// Currently not used:
QString invert_first_author_if_necessary(QString authors){
    QString first_author = prepose_surname_if_necessary(get_first_author(authors));
    if (authors.contains(" and ")){
       QStringList author_list = authors.split("and");
       author_list.replace(0,first_author);
       return  author_list.join (" and ");
    }
    return first_author;
}
