#ifndef INDUSTRIALMODEL_H
#define INDUSTRIALMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QObject>
#include <QPair>

class IndustrialModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    IndustrialModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    static const QMap<int, QList<QString>> keyTexts;
signals:
    void sumGarage(QString );
private:
    QMap<QModelIndex, QVariant> mData;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // INDUSTRIALMODEL_H
