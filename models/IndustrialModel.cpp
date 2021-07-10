#include "IndustrialModel.h"
#include "ProxyLineModel.h"
#include "ProxyIndustrialModel.h"

const QMap<int, QList<QString>> IndustrialModel::keyTexts
= {
    {0, {"Pном", "Кол-во", "Руст"}},
    {1, {"1) Круглопильный станок", "2) Циркулярная пила", "3) Вентилятор", "4) Компрессор", "5) Кран-балка", "6) Дополнительно 1", "7) Дополнительно 2"}},
    {2, {"1) Токарный станок", "2) Фрезерный станок", "3) Сверлильный станок", "4) Наждак", "5) Сварочный трансформатор", "6) Вентилятор", "7) Дополнительно 1"}},
    {3, {"1) Установка досвечивания", "2) Вентиляция", "3) Насос (устанока топлива)", "4) Привод тележки", "5) Дополнительно 1", "6) Дополнительно 2", "7) Дополнительно 3"}}
};

IndustrialModel::IndustrialModel(QObject *parent):QAbstractTableModel (parent)
{

}

int IndustrialModel::rowCount(const QModelIndex &parent) const
{
    return 7 * 3;
}

int IndustrialModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant IndustrialModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        auto it = mData.find(index);
        if(it != mData.end())
        {
            return it.value();
        }
    }
    return QVariant();
}

bool IndustrialModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }
    if(role == Qt::EditRole )
    {   auto v = value.toString();

        if( value.toString() != "")
        {

            mData.insert(index, value);
            emit dataChanged(createIndex(0,2), createIndex(21,2));//index, index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags IndustrialModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return /*index.isValid() ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) :*/ flags;
}

QVariant IndustrialModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        if(section >= 3)
        {
            return keyTexts[0].at(section);
        }
       return keyTexts[0].at(section);
    }
    else
    {
        return keyTexts[1].at(section % 7);
    }

    return QVariant();
}
