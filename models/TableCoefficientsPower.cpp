#include "TableCoefficientsPower.h"
#include <QDateTime>
#include <QTime>
#include <QColor>
#include <QBrush>
TableCoefficientsPower::TableCoefficientsPower(QObject *parent)
{

}

int TableCoefficientsPower::rowCount(const QModelIndex &parent) const
{
    return 6;
}

int TableCoefficientsPower::columnCount(const QModelIndex &parent) const
{
    return 24;
}

QVariant TableCoefficientsPower::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole :
        {
            auto it = mData.find(index);
            if(it != mData.end())
            {
                return it.value();
            }
            else {
                return QVariant();
            }
        }
        case  Qt::TextAlignmentRole : return Qt::AlignCenter;

        default: return QVariant();
    }
}

bool TableCoefficientsPower::setData(const QModelIndex &index, const QVariant &value, int role)
{
    mData.insert(index, value);
    emit dataChanged(createIndex(0,23), createIndex(23,23));
    return true;
}

QVariant TableCoefficientsPower::headerData(int section, Qt::Orientation orientation, int role) const
{

    if( role != Qt::DisplayRole)
    {
            return QVariant();
    }

    if(orientation == Qt::Orientation::Horizontal /*&& section < 1*/)
    {
        return QString(std::to_string(section +  1).c_str() + QString(" ч"));

    }
    else if(orientation == Qt::Orientation::Vertical)
    {
        return section == 0 ? QString("лето") : QString("зима");
    }

    return QVariant();
}

