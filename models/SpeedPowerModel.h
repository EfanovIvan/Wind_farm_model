#ifndef SPEEDPOWERMODEL_H
#define SPEEDPOWERMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class SpeedPowerModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum Dependence
    {
        WG1,
        WG2
    };
    explicit SpeedPowerModel(QObject *parent = nullptr);
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QMap<int, double> getSpeedDependence();
private:
 //   void loadDataFromJson();
signals:

public slots:

private:
    QVector<int>   mSpeeds;
    QMap<QModelIndex, double> mData;
};

#endif // SPEEDPOWERMODEL_H
