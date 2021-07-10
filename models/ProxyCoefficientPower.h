#ifndef PROXYCOEFFICIENTPOWER_H
#define PROXYCOEFFICIENTPOWER_H
#include <QIdentityProxyModel>

class ProxyCoefficientPower: public QIdentityProxyModel
{
    Q_OBJECT
public:
    ProxyCoefficientPower(QAbstractItemModel * sourceModel, int indexRow, QObject *parent = 0);
    enum TypeCoefficient
    {
        SUMMER = 0,
        WINTER
    };

signals:
    void drawIndustrialGraph(QVector<double>, int) const;


public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    int mIndexRow;

};

#endif // PROXYCOEFFICIENTPOWER_H
