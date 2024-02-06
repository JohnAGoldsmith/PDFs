#ifndef FILE_MODEL_H
#define FILE_MODEL_H
#include <QAbstractTableModel>
#include "widget.h"


class FileModel: public QAbstractListModel
{
    Q_OBJECT
    File*          m_file;
    //QStringList    m_bibliography_labels;
    Widget *      m_parent;

public:
    FileModel();
    FileModel (File *, Widget* parent);
    ~FileModel();
    int            rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QStringList    get_bibliography_labels() const  { return   m_parent->get_bibliography_labels();}
    int            get_filename_row();
    //bool           setData(const QModelIndex &index, const QVariant & value, int role) override;
    //Qt::ItemFlags  flags(const QModelIndex &index) const override;
    void           display(File* file);
    void           change_file(File*);
    File*          get_file(){return m_file;}
    void           announce_data_change();
};

#endif // FILE_MODEL_H
