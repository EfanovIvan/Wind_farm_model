#include "ProxyIndustrialModel.h"
#include "IndustrialModel.h"
#include <QColor>
#include <QBrush>

ProxyIndustrialModel::ProxyIndustrialModel(QAbstractItemModel *sourceModel, int offset, TypeIndustrialStructure type, QObject *parent):QIdentityProxyModel(parent), mOffset(offset),mTypeStructure(type),mSumPower(0)
{
    setSourceModel(sourceModel);
}

int ProxyIndustrialModel::rowCount(const QModelIndex &parent) const
{
    return 7;//Sorry magic number Sorry magic number I'm in a hurry
}

int ProxyIndustrialModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ProxyIndustrialModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if(role != Qt::DisplayRole)
     {
         return QVariant();
     }

    if(orientation == Qt::Horizontal)
    {
       return IndustrialModel::keyTexts[0].at(section);
    }
    else
    {
        return IndustrialModel::keyTexts[mTypeStructure].at(section % 7);
    }

    return QVariant();
}

QVariant ProxyIndustrialModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
    {
           return Qt::AlignCenter;
    }

    if(role == Qt::DisplayRole )
    {
        return sourceModel()->data(createIndex(index.row() + mOffset, index.column()), role);
    }
    return QVariant();
}

bool ProxyIndustrialModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        sourceModel()->setData(createIndex(index.row() + mOffset, index.column() ), value, role);

        int indexColumn = index.column() == 0 ? 1 : 0;

        auto ind = createIndex(index.row()+ mOffset, indexColumn);
        auto countValue = sourceModel()->data(ind, role).toString();
        countValue.replace(",", ".");
        auto tempValue = value.toString();
        tempValue.replace(",", ".");

        auto resultPower = countValue.toDouble() * tempValue.toDouble();
        sourceModel()->setData(createIndex(index.row() + mOffset, 2 ), resultPower, role);

        double sum = 0;
        for(int i = 0; i < 7; i++)
        {
            auto data = sourceModel()->data(createIndex(i + mOffset, 2)).toString();
            data.replace(",", ".");
            sum += data.toDouble();
        }
        mSumPower = sum;
        emit sumGarage(QString::number(mSumPower, 'f', 2));
        emit changePower(sum, static_cast<int>(mTypeStructure));

        return true;

    }
    return false;
}

Qt::ItemFlags ProxyIndustrialModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractProxyModel::flags(index);
    return index.isValid() && index.column() != 2 ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}
