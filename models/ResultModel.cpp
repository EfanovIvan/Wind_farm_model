#include "ResultModel.h"
#include <QAbstractTableModel>
#include <array>
#include <QFile>

#include <QAxObject>
#include <QtGui>
#include <QAxWidget>
#include <QApplication>
#include <QFile>
#include <QPair>
#include <QIdentityProxyModel>
#include <QVector>
#include <QFileDialog>

#include "xlsx/xlsxdocument.h"


const QMap<int, QString> ResultModel::keyTexts = {
                                                    {0, "Месяц"},
                                                    {1,"Баллы по\n шкале Бофорта"},
                                                    {2,"Ср. скорость\n ветра, м/с"},
                                                    {3,  "Количество\n дней"},
                                                    {4,  "Wдэг, МВт*ч"},
                                                    {5,  "Wвэу, МВт*ч"},
                                                    {6,  "Wакб, МВт*ч"},
                                                  };
const int ResultModel::dataInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const QMap<int, QPair<double, double>> ResultModel::mBeaufortRange = { {0,qMakePair(0.0, 0.2)},
                                                                       {1,qMakePair(0.3, 1.5)},
                                                                       {2,qMakePair(1.8, 3.6)},
                                                                       {3,qMakePair(3.6, 5.8)},
                                                                       {4,qMakePair(5.8, 8.5)},
                                                                       {5,qMakePair(8.5, 11.0)},
                                                                       {6,qMakePair(11.0, 14.0)},
                                                                       {7,qMakePair(14.0, 17.0)},
                                                                       {8,qMakePair(17.0, 21.0)},
                                                                       {9,qMakePair(21.0, 24.4)},
                                                                       {10,qMakePair(24.5, 28.4)},
                                                                       {11,qMakePair(28.5, 32.6)},
                                                                       {12,qMakePair(33.0, 36.0)},
                                                                     };

ResultModel::ResultModel(QObject *parent):QAbstractTableModel(parent)
{
    InitConstData();
}

int ResultModel::rowCount(const QModelIndex &parent) const
{
    return 12;
}

int ResultModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant ResultModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
        case Qt::DisplayRole : return mData[index];//mDataWG.find(mKeyString.find(index.column()).value()).value();
        case  Qt::TextAlignmentRole : return Qt::AlignCenter;


        default: return QVariant();
    }
}

QVariant ResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if( role != Qt::DisplayRole)
    {
            return QVariant();
     }

    if(orientation == Qt::Orientation::Horizontal && section < keyTexts.size())
    {
       return keyTexts[section];
    }
    else if(orientation == Qt::Orientation::Vertical)
    {
        return QVariant();
    }
    return QVariant();
}

bool ResultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        if(value.toInt() >= 0 && value.toInt() <= 12)
        {
            mData.insert(index, value.toInt());
            mData.insert(createIndex(index.row(), index.column() + 1), QString("%1 - %2").arg(mBeaufortRange[value.toInt()].first).arg(mBeaufortRange[value.toInt()].second));
            emit dataChanged(createIndex(0, index.column()), createIndex(12, index.column()));
            return true;
        }

    }
    return false;
}

Qt::ItemFlags ResultModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() && index.column() == 1 /*&& index.row() < 5*/ ? (flags | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled) : flags;
}

void ResultModel::setCalculateData(const std::array<QVector<QVariant>,3> & calculateData)
{
    auto sumWG = 0.0;
    auto sumDiesel = 0.0;
    for(int i = 0; i < 12; i++)
    {
        auto hourInManth = dataInMonth[i] * 24;
        sumWG += calculateData[2][i].toDouble() * hourInManth/* + calculateData[2][i].toDouble() * hourInManth*/;
        sumDiesel += calculateData[1][i].toDouble() * hourInManth;
        mData.insert(createIndex(i, 6), calculateData[0][i].toDouble() * hourInManth);
        mData.insert(createIndex(i, 4), calculateData[1][i].toDouble() * hourInManth);
        mData.insert(createIndex(i, 5), calculateData[2][i].toDouble() * hourInManth);
    }

   emit sumWGEnergy(sumWG);
   emit sumDieselEnergy(sumDiesel);
   emit totalSum(sumWG + sumDiesel);
}

void ResultModel::expotExel()
{
    try
    {
       QString path = QFileDialog::getExistingDirectory(0,QObject::tr("Укажите директорую куда сохранить файл"),QDir::homePath());
       if(!path.isEmpty())
       {
           QXlsx::Document xlsx;
           xlsx.write("A1", "Hello Qt!");
           xlsx.saveAs(path + "/export.xlsx");

           auto currentPath = QDir::currentPath() ;
           QAxObject* excel = new QAxObject( "Excel.Application", 0 );
           QAxObject* workbooks = excel->querySubObject( "Workbooks" );
           QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", path + "/export.xlsx");
           QAxObject* sheets = workbook->querySubObject( "Worksheets" );
           QList<QVariantList> data; //Data list from excel, each QVariantList is worksheet row
             //worksheets count
               int count = sheets->dynamicCall("Count()").toInt();

           count = sheets->property("Count").toInt();
           for (int i=1; i<=count; i++) //cycle through sheets
            {
               //sheet pointer
               QAxObject* sheet = sheets->querySubObject( "Item( int )", i );

               QString tempStr;
               for(int column = 1; column <= 7; column++)
               {
                   auto nameColumn = keyTexts[column -1];
                   QAxObject * cell = sheet->querySubObject("Cells (int, int)",1, column);
                   cell->setProperty("Value", nameColumn);
                   delete cell;
                   for(int row = 2; row < rowCount(createIndex(0,0)) + 2; row++)
                   {
                       QAxObject * cell = sheet->querySubObject("Cells (int, int)",row, column);
                       auto value = QString::number(mData[createIndex(row - 2 , column - 1)].toDouble(), 'f', 2).replace(".", ",");
                       cell->setProperty("Value", value);
                       delete cell;
                   }

               }

               QAxObject * range = sheet->querySubObject("Range(const QVariant&)", QVariant(QString("A1:G17")));
               range->dynamicCall("Select()");
               range->dynamicCall("VerticalAlignment", -4108);
               range->dynamicCall("HorizontalAlignment", -4108);
               QAxObject * sizeColumn = range->querySubObject("Columns");
               sizeColumn->setProperty("ColumnWidth", 17);


               delete sheet;

         }
         workbook->dynamicCall("Close(Boolean)", true);
         excel->dynamicCall("Quit(void)");
        delete excel;
       }
    }
    catch (...)
    {

    }

}

void ResultModel::clearData()
{
    mData.clear();
    InitConstData();
}

QPair<double, double> ResultModel::getBeaufortRange(int numberOfDay)
{
    int point = mData[createIndex(numberOfDay, 1)].toInt();
    return mBeaufortRange[point];
}

QVector<int> ResultModel::getBeaufortBall()
{
    QVector<int> balls;
    for(int i = 0; i < 12; i++)
    {
        balls.push_back(mData[createIndex(i, 1)].toInt());
    }
    return balls;
}

bool ResultModel::beaufortBallSet()
{
    for(int i = 0; i < 12; i++)
    {
       if(mData[createIndex(i, 1)].toString() == "")
       {
           return false;
       }
    }
    return true;
}

void ResultModel::InitConstData()
{
    for(int i = 0; i < 12; i++)
    {
        mData.insert(createIndex(i, 0), i + 1);
        mData.insert(createIndex(i, 3), dataInMonth[i]);
        mData.insert(createIndex(i, 1), "");

    }
}
