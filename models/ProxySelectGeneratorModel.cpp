#include "ProxySelectGeneratorModel.h"

ProxySelectGeneratorModel::ProxySelectGeneratorModel(QAbstractItemModel *sourceModel, int indexImage, int indexText, QObject *parent) : QIdentityProxyModel(parent),mIndexText(indexText),mIndexImage(indexImage)
{
    setSourceModel(sourceModel);
}

void ProxySelectGeneratorModel::changeIndexImage(int index)
{
    auto variant = sourceModel()->data(createIndex(mIndexImage, index));
    emit changeImage(QPixmap(variant.toString()));
    if(index > 0)
    {
        emit changeWarningIcon(QPixmap(":/Resources/Icon/accept.png"));
    }
    else {
        emit changeWarningIcon(QPixmap(":/Resources/Icon/warning.png"));
    }

}

int ProxySelectGeneratorModel::rowCount(const QModelIndex &parent) const
{
    return sourceModel()->columnCount();
}

int ProxySelectGeneratorModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ProxySelectGeneratorModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole)
    {
         return sourceModel()->data(createIndex(mIndexText, index.row()), role);
    }
   return QVariant();
}

