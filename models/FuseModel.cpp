
#include <QAbstractTableModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>
#include <QPixmap>

#include "FuseModel.h"

FuseModel::FuseModel(QObject *parent):QAbstractTableModel(parent)
{
    loadDataFromJson();
}

int FuseModel::rowCount(const QModelIndex &parent) const
{
    return mData.keys().size();
}

int FuseModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant FuseModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QStringList FuseModel::stringKeys()
{
    return QStringList();
}

QString FuseModel::getPathImage(int columm)
{
    return QString();
}

QStringList FuseModel::getFuseList()
{
    QStringList list;
    for(int i = 0; i < mData.keys().size(); i++)
    {
        list.append(mKeyString[i]);
    }
   // return QStringList(mData.keys());
//    for(auto & key : mData.keys())
//    {
//        list.append(key);
    //    }
    return list;
}

QStringList FuseModel::getCurrentMelts(int index)
{
    auto key = mKeyString[index];
    QStringList list;
    for(auto & key : mMeltData[key])
    {
        list.append(key.first);
    }
    return list;
}

QPixmap FuseModel::getImageFuse(int index)
{
    auto key = mKeyString[index];
    auto pathImage = mData[key].at(3).toString();
    return QPixmap(pathImage);
}

QPixmap FuseModel::getImageMelt(int index)
{
    auto key = mKeyString[index];
    auto pathImage = mData[key].at(5).toString();
    return QPixmap(pathImage);
}

int FuseModel::getCurrentFuse(int index)
{
    auto key = mKeyString[index];
    return mData[key].at(1).toInt();
}

int FuseModel::getCurrentMelt(int indexKeyFuse, int indexMelt)
{
    auto key = mKeyString[indexKeyFuse];
    return mMeltData[key].at(indexMelt).second;
}

void FuseModel::loadDataFromJson()
{
    QFile listDiesel( ":/Resources/Fuse/ListFuse.json");
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

    QJsonValue agentsArrayValue = jsonObject.value("Fuses");
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
        document = document.fromJson(unicode.toUtf8());

        dt.append(fileName);
        dt.append(document.object()["I"].toInt());


        QString prefix = ":/Resources/Image_Fuses/";
        QString pathImage = prefix + document.object()["NameImageFuse"].toString();
        dt.append(document.object()["NameImageFuse"].toString());
        dt.append(pathImage);

        QString pathImageMelt = prefix + document.object()["NameImageMelt"].toString();
        dt.append(document.object()["NameImageMelt"].toString());
        dt.append(pathImageMelt);

        mData[fileName] = dt;

        mKeyString[hashKey] = fileName;
        hashKey++;

        QJsonValue agentsArrayMelt = document.object()["MeltInserts"];
        QJsonArray agentsArray = agentsArrayMelt.toArray();
        QList<QPair<QString, int>> meltList;
        foreach (const QJsonValue & melt, agentsArray)
        {
             auto NameMelt = melt.toObject().value("Name").toString();
             auto I = melt.toObject().value("I").toInt();
             meltList.append(qMakePair(NameMelt, I));
        }
        mMeltData[fileName] = meltList;

     }
}
