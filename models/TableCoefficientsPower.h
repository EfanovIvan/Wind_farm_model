#ifndef TABLECOEFFICIENTSPOWER_H
#define TABLECOEFFICIENTSPOWER_H

#include <QObject>
#include <QAbstractTableModel>

class TableCoefficientsPower: public QAbstractTableModel
{
    Q_OBJECT
public:
    TableCoefficientsPower(QObject * parent = nullptr);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  //  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QHash<QModelIndex, QVariant> mData;
};

#endif // TABLECOEFFICIENTSPOWER_H
