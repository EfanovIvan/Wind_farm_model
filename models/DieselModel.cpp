#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>

#include "DieselModel.h"

const QList<QString> DieselModel::textsParam = {"Мощность, кВт",
                                                 "Род тока",
                                                 "Частота вращения, об/мин",
                                                 "Номинальное напряжение, В",
                                                 "Частота тока, Гц",
                                                 "Марка генератотра",
                                                 "Марка дизеля",
                                                 "Расход топлива, кг/час",
                                                "Масса дизель-генератора, кг",
                                           };
DieselModel::DieselModel(QObject * parent) : QAbstractTableModel(parent)
{
    loadDataFromJson();
}

int DieselModel::rowCount(const QModelIndex &parent) const
{
   return  mDataWG.first().size();
}

int DieselModel::columnCount(const QModelIndex &parent) const
{
    return mDataWG.keys().size();
}

QVariant DieselModel::data(const QModelIndex &index, int role) const
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

QVariant DieselModel::findKeyByColumn(QModelIndex index) const
{
    auto it = mDataWG.find(mKeyString.find(index.column()).value());
    return it.value()[index.row()];
}

bool DieselModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

QVariant DieselModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags DieselModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return /*index.isValid() && indexOfColumn == 1 /*&& index.row() < 5*/ /*? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) :*/ flags;
}

void DieselModel::loadDataFromJson()
{
    QFile listDiesel( ":/Resources/Diesel/ListDiesel.json");
    if( !listDiesel.open( QIODevice::ReadOnly ) )
    {
        int i = 0;
       //qDedug()<< "Error. File is not open";
    }

    QByteArray data = listDiesel.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    int i = 0;
    QJsonObject jsonObject = document.object();

    QJsonValue agentsArrayValue = jsonObject.value("List");
    QJsonArray agentsArray = agentsArrayValue.toArray();

    int hashKey = 0;
    foreach (const QJsonValue & v, agentsArray)
    {
        QFile file(v.toObject().value("path").toString());
        if( !file.open( QIODevice::ReadOnly ) )
        {
            int i = 0;
           //qDedug()<< "Error. File is not open";
        }


        auto fileName = v.toObject().value("path").toString();
        QStringList fileSplit = fileName.split("/");
        QString temp = fileSplit.at(fileSplit.size()-1);
        fileName = temp.section(".",0,0);

        QList<QVariant> dt;

        QByteArray data = file.readAll();
        QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
        QString unicode = defaultTextCodec->toUnicode(data);
        QJsonDocument document;
        document = document.fromJson( unicode.toUtf8() );

        dt.append(document.object()["P"].toInt());
        dt.append(document.object()["CurrentType"].toString());
        dt.append(document.object()["RotationFrequency"].toInt());
        dt.append(document.object()["U"].toInt());
        dt.append(document.object()["F"].toInt());

        dt.append(document.object()["MarkGenerators"].toString());
        dt.append(document.object()["MarkDiesel"].toString());
        dt.append(document.object()["FuelConsumption"].toInt());
        dt.append(document.object()["weight"].toInt());
        dt.append(fileName);

        QString prefix = ":/Resources/Image_Diesel/";
        QString pathImage = prefix + document.object()["nameImage"].toString();
        dt.append(document.object()["nameImage"].toString());
        dt.append(pathImage);
        mDataWG[fileName] = dt;

        mKeyString[hashKey] = fileName;
        hashKey++;

     }
}
