#ifndef PROXYLINEMODEL_H
#define PROXYLINEMODEL_H

#include <QIdentityProxyModel>
class ProxyLIneModel: public QIdentityProxyModel
{
    Q_OBJECT

public:
    enum OffsetLines
    {
        RESIDENTIAL_LINE = 0,
        LIVESTOCK_LINE = 2,
        INDUSTRIAL_LINE = 4
    };

     ProxyLIneModel(QAbstractItemModel * sourceModel, int column, OffsetLines numberLine, QObject *parent = nullptr);
     void setColumn(int column) { mColumn = column; }
    double getCosfi();
    double getLengthLine();
    int getIlongValid(int row);
    int getNullWireSection(int row);
signals:
    void dataChanged(int row);

public slots:
     //void updateView();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    static const QList<QString> textsParam;
    int mColumn;
    OffsetLines mOffsetLine;
    int mValue;

};

#endif // PROXYLINEMODEL_H
