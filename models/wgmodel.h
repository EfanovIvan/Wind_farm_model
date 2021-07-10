#ifndef WGMODEL_H
#define WGMODEL_H

#include <QAbstractTableModel>
#include<QObject>

class WGModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WGModel(QObject * parent = nullptr);

public:

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QStringList stringKeys();
    QString getPathImage(int columm);
    QMap<int, double> getSpeedPowerDependence();

private:
    static const QList<QString> textsParam;
    QVariant findKeyByColumn(QModelIndex index) const;
    void loadDataFromJson();
    QList<int> rowIndex;
    QMap<QString, QList<QVariant>>  mDataWG;
    QHash<int, QString> mKeyString;
    QMap<int, double> mSpeedPowerDependence;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // WGMODEL_H
