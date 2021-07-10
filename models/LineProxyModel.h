 #ifndef LINEPROXYMODEL_H
#define LINEPROXYMODEL_H

#include <QObject>
#include <QAbstractProxyModel>

class LineProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    LineProxyModel(QObject *parent = nullptr);
public slots:
    void setColumn(int column) { mColumn = column; }

public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

 private:
   int mColumn;
   static const QList<QString> textsParam;
};

#endif // LINEPROXYMODEL_H
