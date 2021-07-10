#ifndef RESULTMODELL_H
#define RESULTMODELL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPair>

class ResultModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ResultModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setCalculateData(const std::array<QVector<QVariant>,3> & calculateData);
    void clearData();
    QPair<double, double> getBeaufortRange(int numberOfDay);
    static const int dataInMonth[12];
    QVector<int> getBeaufortBall();
    bool beaufortBallSet();
    //int countRow();
signals:
    void sumWGEnergy(double sum);
    void sumDieselEnergy(double sum);
    void totalSum(double);

public slots:
    void expotExel();
private:
    void InitConstData();

    static const QMap<int, QString> keyTexts;
    static const QMap<int, QPair<double, double>> mBeaufortRange;

    QList<QVariant>             mCalculatedData;
    QMap<QModelIndex, QVariant> mData;
};

#endif // RESULTMODELL_H
