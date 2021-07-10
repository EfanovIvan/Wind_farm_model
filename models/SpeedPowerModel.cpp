#include "SpeedPowerModel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

SpeedPowerModel::SpeedPowerModel(QObject *parent) : QAbstractTableModel(parent)
{
    for(int i = 0; i < 23; i++)
    {
        mSpeeds.push_back(i+3);
    }

}

int SpeedPowerModel::rowCount(const QModelIndex &parent) const
{
    return 23;
}

int SpeedPowerModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SpeedPowerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole :
    {
        if(index.column() == 0)
        {
            return mSpeeds[index.row()];
        }
        else
        {
            return mData[index];
        }
    }
        case  Qt::TextAlignmentRole : return Qt::AlignCenter;


        default: return QVariant();
    }
}

bool SpeedPowerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        mData.insert(index, value.toString().replace(",",".").toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant SpeedPowerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Orientation::Horizontal)
    {
       return section == 0 ? "Скорость ветра,\n м/с" : "Мощность, кВт";
    }
    else if(orientation == Qt::Orientation::Vertical)
    {
        return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags SpeedPowerModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() && index.column() == 1 ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}

QMap<int, double> SpeedPowerModel::getSpeedDependence()
{
    QMap<int, double> dependece;
    for(int i = 0; i < 23; i++)
    {
        dependece.insert(i+3, mData[createIndex(0, i)]);
    }
    return dependece;
}




