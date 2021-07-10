#ifndef WGPROXYMODEL_H
#define WGPROXYMODEL_H

#include <QIdentityProxyModel>
#include <QObject>
#include <QMessageBox>
#include <memory>

class WGProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:

    enum TypeGenerators
    {
        WG = 0,
        Diesel
    };

    WGProxyModel(QAbstractItemModel * sourceModel, QObject *parent = 0, TypeGenerators type = WG);

    struct DataWG
    {
        DataWG():wheelDiameter(0.0),nomPower(0),cos(0),isSelected(false){}
        DataWG(double wheelDiam, int P, double cos_, bool _isSelected): wheelDiameter(wheelDiam),nomPower(P),cos(cos_),isSelected(_isSelected){}
        double wheelDiameter;
        double cos;
        double nomPower;
        bool isSelected;
        QPair<double, double> speedRange;
        QMap<int, double> speedPowerDependence;
    };

signals:
    void changeImage(int inedexOfColumn);
public  slots:
    void chageIndexColumn(int index);
    // QAbstractItemModel interface
public:

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void loadDataFromFile(const QString path, const QString nameGenerator);
private:
    int indexOfColumn;
    TypeGenerators mTypeGenerators;
    std::shared_ptr<QMessageBox> messageBox;
    // QAbstractItemModel interface
public:
    DataWG getDataWG();
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // WGPROXYMODEL_H
