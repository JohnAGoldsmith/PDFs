#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H
#include <QAbstractListModel>
#include <QWidget>


class Entry;


class EntryModel : public QAbstractListModel{
    //friend class Widget;

    Q_OBJECT
    Entry*          m_entry;
    QStringList    m_bibliography_labels;
    QWidget         m_parent;

public:
                   EntryModel (Entry *, QList<QString> bibliography_labels);
                   ~EntryModel();
    int            rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int            columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant       data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QStringList    get_bibliography_labels() const  { return   m_bibliography_labels;}
    int             get_filename_row();
    bool           setData(const QModelIndex &index, const QVariant & value, int role) override;
    Qt::ItemFlags  flags(const QModelIndex &index) const override;
    void           display(Entry* entry);
    void           change_entry(Entry*);
    Entry*          get_entry(){return m_entry;}
//    void           dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
//    void           setData(Entry* entry);
//    bool           setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};

#endif // ENTRYMODEL_H
