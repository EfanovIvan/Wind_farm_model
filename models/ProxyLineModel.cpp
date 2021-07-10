#include "ProxyLineModel.h"
#include "LinesMode.h"

const QList<QString> ProxyLIneModel::textsParam {"Длина линии, м", QString("Коэффициент нагрузки линии, cos %1").arg(QChar(0x03C6)) };

//ProxyLIneModel::ProxyLIneModel(QObject *parent):QIdentityProxyModel(parent), mColumn(0)
//{

//}

ProxyLIneModel::ProxyLIneModel(QAbstractItemModel *sourceModel, int column, ProxyLIneModel::OffsetLines numberLine, QObject *parent):QIdentityProxyModel(parent), mColumn(column), mOffsetLine(numberLine)
{
    if(sourceModel == nullptr)
    {
        throw("LOGIC EROOR");
    }
    this->setSourceModel(sourceModel);
}

double ProxyLIneModel::getCosfi()
{
    if(mColumn == 2)
    {
        auto val = sourceModel()->data(createIndex(1 + mOffsetLine, mColumn),Qt::DisplayRole);
        return val.toDouble();
    }
    else
    {
        return 0.0;
    }
}

double ProxyLIneModel::getLengthLine()
{
    if(mColumn == 2)
    {
        auto val = sourceModel()->data(createIndex(0 + mOffsetLine, mColumn),Qt::DisplayRole);
        return val.toDouble();
    }
    else
    {
        return 0.0;
    }
}

int ProxyLIneModel::getIlongValid(int row)
{
    auto val = sourceModel()->data(createIndex(row /*+ mOffsetLine*/, mColumn), Qt::DisplayRole);
    return val.toInt();
}

int ProxyLIneModel::getNullWireSection(int row)
{
    auto val = sourceModel()->data(createIndex(row /*+ mOffsetLine*/, 1), Qt::DisplayRole);
    return val.toInt();
}

//void ProxyLIneModel::changeValue(int indexData)
//{
//    mValue = sourceModel()->data(createIndex(indexData + mOffsetLine, mColumn), Qt::DisplayRole).toInt();
//}

int ProxyLIneModel::rowCount(const QModelIndex &parent) const
{
    if(mColumn == 2)
    {
        return 2;
    }
    int i = sourceModel()->rowCount();
    return  i;
}


int ProxyLIneModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ProxyLIneModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole /*|| role == Qt::EditRole*/)
    {
        if(mColumn > 1)
        {
           return sourceModel()->data(createIndex(index.row() + mOffsetLine, mColumn), role);
        }
        auto v = sourceModel()->data(createIndex(index.row(), mColumn), role);
        auto var = LinesMode::keyTexts[v.toInt()];
        return QVariant(var);
    }
    return QVariant();
}

bool ProxyLIneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole /*&& index.column() > 1*/)
    {
        sourceModel()->setData(createIndex(index.row() + mOffsetLine, mColumn ), value, role);
        emit dataChanged(mColumn != 2 ? index.row() : -1);
        return true;
    }
    return false;
}

QVariant ProxyLIneModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags ProxyLIneModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractProxyModel::flags(index);
    return index.isValid() && mColumn == 2 && index.row() < 2 ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}
