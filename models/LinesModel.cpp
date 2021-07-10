
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>

#include "utils/DigitalValidator.h"
#include "models/LinesModel.h"

const QMap<int, QString> LinesMode::keyTexts = {
                                                    {0, "Убрать линию"},
                                                    {105,"Провод марки А16 (Iд.доп=105А)"},
                                                    {135,  "Провод марки Е25 (Iд.доп=135А)"},
                                                    {170,  "Провод марки А35 (Iд.доп=170А)"},
                                                    {215,  "Провод марки А50 (Iд.доп=215А"},
                                                    {265, "Провод марки А70 (Iд.доп=265А)"},
                                                    {320,  "Провод марки А95 (Iд.доп=320А)"},
                                                    {100,  "СИП-2 3х16+1х25"   },
                                                    {130,  "СИП-2 3х25+1х35"   },
                                                    {160,  "СИП-2 3х35+1х50"   },
                                                    {195,  "СИП-2 3х50+1х50"   },
                                                    {240,  "СИП-2 3х70+1х70"   },
                                                    {300,  "СИП-2 3х95+1х70"   },
                                                    {340,  "СИП-2 3х120+1х95"  },
                                                    {16,  "Провод марки А16"},
                                                    {25,  "Провод марки Е25"},
                                                    {35,  "Провод марки А35"},
                                                    {50,  "Провод марки А50"},
                                                    {70,  "Провод марки А70"},
                                                    {95,  "Провод марки А95"},
                                               };
LinesMode::LinesMode(QObject *parent) : QAbstractTableModel(parent)
{
    loadDataFromJson();
    validator = std::make_shared<DigitalValidator>();
}


int LinesMode::rowCount(const QModelIndex &parent) const
{
    return mData.size();
}

int LinesMode::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant LinesMode::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return mData[index.row()].name;
    }
    return QVariant();
}

bool LinesMode::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }
//    if(role == Qt::EditRole /*&& index.column() > 1*/)
//    {   auto v = value.toString();
//        int i = 0;
//        if(/*validator->validate(v, i) == QValidator::Acceptable &&*/ value.toString() != "")
//        {
//            auto it = mData.find(index.column());
//            if(it != mData.end())
//            {
//                if(index.row() % 2 == 0)
//                {

//                        it.value()[index.row()] = value.toInt();it.value()[index.row()] = value.toDouble();

//                }
//                else
//                {
//                    if(value.toDouble() <= 1)
//                    {
//                        it.value()[index.row()] = value.toDouble();
//                    }
//                    else
//                    {
//                        return false;
//                    }
//                }

//                emit dataChanged(createIndex(0,3), createIndex(6,3));//index, index);
//                emit updateView();
//                return true;
//              // auto val = mData[1/*index.column()*/].at(index.row());
//            }
//        }

//    }
    return false;
}

void LinesMode::loadDataFromJson()
{

    QFile listDiesel( ":/Resources/Diesel/ListOfWire.json");
    if( !listDiesel.open( QIODevice::ReadOnly ) )
    {
        int i = 0;
       //qDedug()<< "Error. File is not open";
    }

    QList<DataWire> listDataWires;

    QByteArray data = listDiesel.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    int i = 0;
    QJsonObject jsonObject = document.object();

    QJsonValue agentsArrayValue = jsonObject.value("Wires");
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

        QByteArray data = file.readAll();
        QJsonDocument document;
        document = document.fromJson( data );

        DataWire dw;
        dw.name = document["name"].toString();
        dw.Ilong = document["I"].toInt();
        dw.r = document["r"].toString().toDouble();
        dw.x = document["x"].toString().toDouble();
        dw.r0 = document["r0"].toString().toDouble();
        dw.x0 = document["x0"].toString().toDouble();

       // listDataWires.append(dw);

        mData.insert(hashKey, dw);

        hashKey++;

     }

}
