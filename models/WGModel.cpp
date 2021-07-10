#include "WGModel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
const QList<QString> WGModel::textsParam = {"Номинальная мощность, кВт",
                                                 "Коэффициент использования\n энергии ветра (КИЭВ)",
                                                 "Диаметр ветроколеса, м",
                                                 "Число лопастей, n",
                                                 "Ном. частота вращения, об/мин",
                                                 "Диапазон рабочих скоростей\nНоминальная скорость, м/с",
                                                 "Минимальная скорость, м/с",
                                                 "Максимальная скорость, м/с",
                                                "Номинальное напряжение, В",
                                                "Номинальная частота, Гц",
                                                QString("Площадь ометаемой поверхности, м%1").arg(QChar(0x00B2)),
                                                "Высота установки гондолы ВЭУ,м",
                                                "Тип генератора",
                                                "Возможность работы с сетью"
                                           };

WGModel::WGModel(QObject * parent):QAbstractTableModel(parent)
{
    loadDataFromJson();
}

int WGModel::rowCount(const QModelIndex &parent) const
{
    return mDataWG.first().size();
}

int WGModel::columnCount(const QModelIndex &parent) const
{
    return mDataWG.keys().size();
}

QVariant WGModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole : return findKeyByColumn(index);//mDataWG.find(mKeyString.find(index.column()).value()).value();
        case  Qt::TextAlignmentRole : return Qt::AlignCenter;

        default: return QVariant();
    }
}

QVariant WGModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if( role != Qt::DisplayRole)
    {
            return QVariant();
      }

    if(orientation == Qt::Orientation::Horizontal /*&& section < 1*/)
    {
        return QString("Значение");
    }
    else if(orientation == Qt::Orientation::Vertical && section < textsParam.size())
    {
        return textsParam[section];
    }

    return QVariant();
}

QStringList WGModel::stringKeys()
{
    QStringList lis;
    for(int i = 0; i < mDataWG.size(); i++)
    {
        lis << mKeyString[i];
    }
    return lis;//mKeyString.values();
}

QString WGModel::getPathImage(int columm)
{
    auto key = mKeyString[columm];
    return mDataWG[key].takeAt(14).toString();
}

QMap<int, double> WGModel::getSpeedPowerDependence()
{
    return mSpeedPowerDependence;
}

QVariant WGModel::findKeyByColumn(QModelIndex index) const
{
    auto it = mDataWG.find(mKeyString.find(index.column()).value());
    return it.value()[index.row()];
}

void WGModel::loadDataFromJson()
{
    QFile listWGs( ":/Resources/DataWG/ListOfWindGenerator.json");
        if( !listWGs.open( QIODevice::ReadOnly ) )
        {
           //TODO
            return;
        }



        QByteArray data = listWGs.readAll();
        QJsonDocument document;
        document = document.fromJson( data);

        int i = 0;
        QJsonObject jsonObject = document.object();

        QJsonValue agentsArrayValue = jsonObject.value("WGs");
         QJsonArray agentsArray = agentsArrayValue.toArray();

         int hashKey = 0;
        foreach (const QJsonValue & v, agentsArray)
        {
            QFile file(v.toObject().value("path").toString());
            if( !file.open( QIODevice::ReadOnly ) )
            {
                //TODO
                 return;
            }


            auto fileName = v.toObject().value("path").toString();
            QStringList fileSplit = fileName.split("/");
            QString temp = fileSplit.at(fileSplit.size()-1);
            fileName = temp.section(".",0,0);
            if(fileName == "Other")
            {
                fileName = "Другой тип ВЭУ";
            }
            QList<QVariant> dt;

            QByteArray data = file.readAll();
            QJsonDocument document;
            document = document.fromJson( data );

            dt.append(document.object()["P"].toInt());
            dt.append(document.object()["Cos"].toString().toFloat());
            dt.append(document.object()["WheelDiameter"].toString().toFloat());
            dt.append(document.object()["Vanes"].toInt());
            dt.append(document.object()["RotationFrequency"].toInt());

            QJsonObject document2 = document.object()["OperatingSpeedRange"].toObject();
             dt.append(document2["nom"].toString().toFloat());
             dt.append(document2["min"].toString().toFloat());
             dt.append(document2["max"].toInt());


            dt.append(document.object()["U"].toInt());
            dt.append(document.object()["F"].toInt());
            dt.append(document.object()["SurfaceArea"].toInt());
            dt.append(document.object()["InstallationHeight"].toString().toFloat());

            dt.append(document.object()["TypeWG"].toBool() == true ? "синхронный": "асинхронный");
            dt.append(document.object()["WorkWithNetwork"].toBool() == true ? "есть" : "нет");

            dt.append(fileName);
            QString prefix = ":/Resources/Image_WG/";
            QString pathImage = prefix + document.object()["nameImage"].toString();
            dt.append(document.object()["nameImage"].toString());
            dt.append(pathImage);
            mDataWG[fileName] = dt;

            mKeyString[hashKey] = fileName;
            hashKey++;

        }
}

QModelIndex WGModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

bool WGModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        auto it = mDataWG.find(mKeyString.find(index.column()).value());
        if(it != mDataWG.end())
        {
            it.value()[index.row()] = value;
            emit dataChanged(createIndex(0, index.column()), createIndex(14, index.column()));
            return true;
        }
    }
    return false;
}
