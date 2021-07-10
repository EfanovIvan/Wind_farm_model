#include "WGProxyModel.h"
#include <QIdentityProxyModel>
#include <QFile>
#include <QTextCodec>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "WGModel.h"

WGProxyModel::WGProxyModel(QAbstractItemModel * sourceModel, QObject *parent, TypeGenerators type) : QIdentityProxyModel(parent), indexOfColumn(0),mTypeGenerators(type)
{
    setSourceModel(sourceModel);
}

void WGProxyModel::chageIndexColumn(int index)
{
    indexOfColumn = index;
    emit changeImage(indexOfColumn);
    emit dataChanged(sourceModel()->index(0, 0), sourceModel()->index(14, indexOfColumn));
}

int WGProxyModel::rowCount(const QModelIndex &parent) const
{
    return sourceModel()->rowCount() - 3;
}

int WGProxyModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void WGProxyModel::loadDataFromFile(const QString path, const QString nameGenerator)
{
    QFile file(path);
    if( !file.open( QIODevice::ReadOnly ) )
    {
        messageBox = std::make_shared<QMessageBox>();
        messageBox->setText(QString("Невозможно открыть файл"));
        messageBox->show();
    }

    QByteArray data = file.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    QJsonObject objectWG = document.object()[nameGenerator.toLocal8Bit().data()].toObject();
    chageIndexColumn(objectWG["index"].toInt());
}

WGProxyModel::DataWG WGProxyModel::getDataWG()
{
    if(mTypeGenerators == WG)
    {
        QMap<int, double> speedPowerDependence;
        if(auto cast = qobject_cast<WGModel*>(sourceModel()))
        {
            speedPowerDependence = cast->getSpeedPowerDependence();
        }
        auto diam = sourceModel()->data(createIndex(2, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");
        auto power = sourceModel()->data(createIndex(0, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");;
        auto cos = sourceModel()->data(createIndex(1, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");;
        auto minWindSpeed = sourceModel()->data(createIndex(6, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");;
        auto maxWindSpeed = sourceModel()->data(createIndex(7, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");;
        auto dataWG = DataWG(diam.toDouble(), power.toDouble() * 1000, cos.toDouble(), indexOfColumn != 0);
        dataWG.speedRange = qMakePair(minWindSpeed.toDouble(), maxWindSpeed.toDouble());
        dataWG.speedPowerDependence = speedPowerDependence;
        return dataWG;
    }
    else
    {
        auto power = sourceModel()->data(createIndex(0, indexOfColumn), Qt::DisplayRole).toString().replace(",",".");
        return DataWG(0, power.toDouble() * 1000, 0, indexOfColumn != 0 );
    }
}

QVariant WGProxyModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole)
    {
        return sourceModel()->data(createIndex(index.row(), indexOfColumn), role);
    }
    return QVariant();
}

bool WGProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        sourceModel()->setData(createIndex(index.row(), indexOfColumn), value, role);
    }
    return false;
}

Qt::ItemFlags WGProxyModel::flags(const QModelIndex &index) const
{
    if(mTypeGenerators == TypeGenerators::WG)
    {
        Qt::ItemFlags flags = QIdentityProxyModel::flags(index);
        return index.isValid() && indexOfColumn == 1 && (index.row() <= 8 ||  index.row() == 11 || index.row() == 10)? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
    }
    else
    {
        Qt::ItemFlags flags = QIdentityProxyModel::flags(index);
        return index.isValid() && indexOfColumn == 1 && index.row() == 0? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
    }

}


