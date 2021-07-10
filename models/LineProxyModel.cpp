#include "LineProxyModel.h"
#include "LinesMode.h"

const QList<QString> LineProxyModel::textsParam {"Введите длину линии, м", "Введите cos нагрузки линии" };

LineProxyModel::LineProxyModel(QObject *parent) : QAbstractProxyModel(parent),mColumn(0)
{

}


QModelIndex LineProxyModel::index(int row, int column, const QModelIndex &parent) const
{
     return createIndex(row, column );
}

QModelIndex LineProxyModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int LineProxyModel::rowCount(const QModelIndex &parent) const
{
    if(mColumn == 2)
    {
        return 2;
    }
    int i = sourceModel()->rowCount();
    return  i;
}

int LineProxyModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex LineProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
          return QModelIndex();
    return sourceModel()->index(proxyIndex.row() , mColumn);
}


QModelIndex LineProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
//    if(sourceIndex.column() > 1)
//    {
//        sourceModel()->setData(sourceIndex, 1, Qt::EditRole);
//    }
    return sourceIndex;

}

QVariant LineProxyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole /*|| role == Qt::EditRole*/)
    {
        if(mColumn > 1)
        {
           return sourceModel()->data(createIndex(index.row(), mColumn), role);
        }
        auto v = sourceModel()->data(createIndex(index.row(), mColumn), role);
        auto var = LinesMode::keyTexts[v.toInt()];
        return QVariant(var);
    }
    return QVariant();
}

bool LineProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole && index.column() > 1)
    {
        sourceModel()->setData(index, value, role);
    }
    return false;
}

QVariant LineProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole)
    {
            return QVariant();
     }

    if(mColumn > 1)
    {
        if(orientation == Qt::Orientation::Vertical && section < textsParam.size())
        {
            return textsParam[section];
        }
    }
    return QVariant();
}

Qt::ItemFlags LineProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractProxyModel::flags(index);
    return index.isValid() && mColumn == 2 && index.row() < 2 ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}
