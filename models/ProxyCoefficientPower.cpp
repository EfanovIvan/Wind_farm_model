#include "Proxycoefficientpower.h"
#include <QString>


ProxyCoefficientPower::ProxyCoefficientPower(QAbstractItemModel *sourceModel, int indexRow, QObject *parent):QIdentityProxyModel(parent), mIndexRow(indexRow)
{
    setSourceModel(sourceModel);
}

int ProxyCoefficientPower::rowCount(const QModelIndex &parent) const
{
    return 2;
}

int ProxyCoefficientPower::columnCount(const QModelIndex &parent) const
{
    return sourceModel()->columnCount();
}

QVariant ProxyCoefficientPower::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole :
        {
            if(index.row() == 0)
            {
                return sourceModel()->data(createIndex(mIndexRow, index.column()), role);
            }
            else
            {
                  return sourceModel()->data(createIndex(mIndexRow + 3, index.column()), role);
            }

        }
        case  Qt::TextAlignmentRole : return Qt::AlignCenter;
        case  Qt::ToolTipRole : return QString("Значение коэффициента должно быть в диапозоне 0 %1x%2 1").arg(QChar(0x2264)).arg(QChar(0x2265));

        default: return QVariant();
    }
}

bool ProxyCoefficientPower::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
       QVector<double> k(24);
       sourceModel()->setData(createIndex(index.row() == 0 ? mIndexRow : mIndexRow + 3, index.column()), value, role);
       for(int i = 0; i < sourceModel()->columnCount(); i++)
       {
           auto value = sourceModel()->data(createIndex(index.row() == 0 ? mIndexRow : mIndexRow + 3, i), Qt::DisplayRole).toString();
           value.replace(",", ".");
           k[i] = value.toDouble();//sourceModel()->data(createIndex(index.row() == 0 ? mIndexRow : mIndexRow + 3, i), Qt::DisplayRole).toString().toDouble();
       }
       emit drawIndustrialGraph(k, index.row() == 0 ? SUMMER : WINTER);
    }
    return false;
}

Qt::ItemFlags ProxyCoefficientPower::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QIdentityProxyModel::flags(index);
    return index.isValid()  ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}
