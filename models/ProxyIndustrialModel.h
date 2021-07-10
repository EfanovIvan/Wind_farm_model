#ifndef PROXYINDUSTRIALMODEL_H
#define PROXYINDUSTRIALMODEL_H

#include <QObject>
#include <QIdentityProxyModel>
class ProxyIndustrialModel: public QIdentityProxyModel
{
    Q_OBJECT
public:
    enum TypeIndustrialStructure
    {
        GARAGE = 1,
        MANUFACTORY = 2,
        GREENHOUSE = 3
    };
    ProxyIndustrialModel(QAbstractItemModel * sourceModel, int offset, TypeIndustrialStructure type,  QObject *parent = nullptr);
    double getSupPower() {return mSumPower;}
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
signals:
    void sumGarage(QString );
    void changePower(double, int);
private:
    int mOffset;
    TypeIndustrialStructure mTypeStructure;
    double mSumPower;
};

#endif // PROXYINDUSTRIALMODEL_H
