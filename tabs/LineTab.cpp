
#include <QIdentityProxyModel>
#include <QPixmap>
#include <math.h>
#include <QMessageBox>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextCodec>
#include <QStringListModel>

#include "utils/DoubleValidator.h"
#include "tabs/TabIndex.h"
#include "tabs/LineTab.h"
#include "ui_linetab.h"
#include "models/LinesModel.h"
#include "models/LineProxyModel.h"
#include "models/ProxyLineModel.h"
#include "tabs/TabIndex.h"

LineTab::LineTab(QWidget *parent) : QWidget(parent), ui(new Ui::Form), mCurrentLine(NONE),mDelUWG(0.0)
{
    ui->setupUi(this);
    qRegisterMetaType<LineTab::DataLine>( "LineTab::DataLine" );
    loadDataFromJson();
    setTextLable();

    ui->comboBox1Line1->setModel(new QStringListModel(getNamesWires() , this));
    ui->comboBox2Line1->setModel(new QStringListModel(getNamesNullWires() , this));

    ui->comboBox1Line2->setModel(new QStringListModel(getNamesWires() , this));
    ui->comboBox2Line2->setModel(new QStringListModel(getNamesNullWires() , this));

    ui->comboBox1Line3->setModel(new QStringListModel(getNamesWires() , this));
    ui->comboBox2Line3->setModel(new QStringListModel(getNamesNullWires() , this));

    ui->comboBoxWG1->setModel(new QStringListModel(getNamesWiresWG() , this));
    ui->comboBoxWG2->setModel(new QStringListModel(getNamesWiresWG() , this));

    auto validator = new DoubleValidator(0.0, 1.0, this);
    ui->cos1->setValidator(validator);
    ui->cos2->setValidator(validator);
    ui->cos3->setValidator(validator);

    auto lengthValidator = new DoubleValidator(0.0, 1000.0, this);
    ui->length1->setValidator(lengthValidator);
    ui->length2->setValidator(lengthValidator);
    ui->length3->setValidator(lengthValidator);

    QObject::connect(ui->cos1, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->cos2, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->cos3, SIGNAL(editingFinished()), this, SLOT(recountLine()));

    QObject::connect(ui->length1, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->length2, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->length3, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->wg1LineEdit, SIGNAL(editingFinished()), this, SLOT(recountLine()));
    QObject::connect(ui->wg2LineEdit, SIGNAL(editingFinished()), this, SLOT(recountLine()));

    QObject::connect(ui->comboBox1Line1, SIGNAL(currentIndexChanged(QString)), this, SLOT(recountLine(QString)));
    QObject::connect(ui->comboBox2Line1, SIGNAL(currentIndexChanged(QString)), this, SLOT(setIndexLine(QString)));

    QObject::connect(ui->comboBox1Line2, SIGNAL(currentIndexChanged(QString)), this, SLOT(recountLine(QString)));
    QObject::connect(ui->comboBox2Line2, SIGNAL(currentIndexChanged(QString)), this, SLOT(setIndexLine(QString)));

    QObject::connect(ui->comboBox1Line3, SIGNAL(currentIndexChanged(QString)), this, SLOT(recountLine(QString)));
    QObject::connect(ui->comboBox2Line3, SIGNAL(currentIndexChanged(QString)), this, SLOT(setIndexLine(QString)));

    QObject::connect(ui->comboBoxWG1, SIGNAL(currentIndexChanged(QString)), this, SLOT(recountLine(QString)));
    QObject::connect(ui->comboBoxWG2, SIGNAL(currentIndexChanged(QString)), this, SLOT(recountLine(QString)));

    mRegistedObject.insert(ui->cos1, LINE_1_RESIDENTIAL);
    mRegistedObject.insert(ui->cos2, LINE_2_LIVESTOCK);
    mRegistedObject.insert(ui->cos3, LINE_3_INDUSTRIAL);

    mRegistedObject.insert(ui->length1, LINE_1_RESIDENTIAL);
    mRegistedObject.insert(ui->length2, LINE_2_LIVESTOCK);
    mRegistedObject.insert(ui->length3, LINE_3_INDUSTRIAL);
    mRegistedObject.insert(ui->wg1LineEdit, LINE_WG1);
    mRegistedObject.insert(ui->wg2LineEdit, LINE_WG2);

    mRegistedObject.insert(ui->comboBox2Line1, LINE_1_RESIDENTIAL);
    mRegistedObject.insert(ui->comboBox2Line2, LINE_2_LIVESTOCK);
    mRegistedObject.insert(ui->comboBox2Line3, LINE_3_INDUSTRIAL);

    mRegistedObject.insert(ui->comboBox1Line1, LINE_1_RESIDENTIAL);
    mRegistedObject.insert(ui->comboBox1Line2, LINE_2_LIVESTOCK);
    mRegistedObject.insert(ui->comboBox1Line3, LINE_3_INDUSTRIAL);
    mRegistedObject.insert(ui->comboBoxWG1, LINE_WG1);
    mRegistedObject.insert(ui->comboBoxWG2, LINE_WG2);

    setObjectLine(LINE_1_RESIDENTIAL, ui->comboBox2Line1, ui->Icount,   ui->delU,   ui->cond1Line1, ui->cond2Line1, ui->chemDelU2_1, ui->cos1, ui->length1, ui->textLIne_1, ui->validLabel_1);
    setObjectLine(LINE_2_LIVESTOCK,   ui->comboBox2Line2, ui->Icount_2, ui->delU_2, ui->cond1Line2, ui->cond2Line2, ui->chemDelU2_2, ui->cos2, ui->length2, ui->textLIne_2, ui->validLabel_2);
    setObjectLine(LINE_3_INDUSTRIAL,  ui->comboBox2Line3, ui->Icount_3, ui->delU_3, ui->cond1Line3, ui->cond2Line3, ui->chemDelU2_3, ui->cos3, ui->length3, ui->textLIne_3, ui->validLabel_3);
    setObjectLine(LINE_WG1,  nullptr, ui->IcountWG1, nullptr, ui->cond1LineWG1, nullptr, ui->chemDelU2_3, nullptr, ui->wg1LineEdit, ui->textLIne_4, ui->validLabel_4);
    setObjectLine(LINE_WG2,  nullptr, ui->IcountWG2, nullptr, ui->cond1LineWG2, nullptr, ui->chemDelU2_3, nullptr, ui->wg2LineEdit, ui->textLIne_5, ui->validLabel_5);

    mDataLines[LINE_WG1].cosFi = 0.9;
    mDataLines[LINE_WG2].cosFi = 0.9;

    ui->comboBoxWG1->setCurrentText("Убрать линию");
    ui->comboBoxWG2->setCurrentText("Убрать линию");
    ui->comboBox1Line1->setCurrentText("Убрать линию");
    ui->comboBox1Line2->setCurrentText("Убрать линию");
    ui->comboBox1Line3->setCurrentText("Убрать линию");
    ui->comboBox2Line1->setCurrentText("Убрать линию");
    ui->comboBox2Line2->setCurrentText("Убрать линию");
    ui->comboBox2Line3->setCurrentText("Убрать линию");
}

void LineTab::setObjectLine(NumberLine line, QComboBox * comBox2, QLabel * Icount, QLabel * delU, QLabel * cond1, QLabel * cond2, QLabel * cond3, QLineEdit * cos, QLineEdit * length, QLabel * text, QLabel * validIcon)
{
    mDataLines[line].comBox2 = comBox2;
    mDataLines[line].Icount  = Icount;
    mDataLines[line].delU    = delU;
    mDataLines[line].cond1   = cond1;
    mDataLines[line].cond2   = cond2;
    mDataLines[line].cond3   = cond3;
    mDataLines[line].uiCos   = cos;
    mDataLines[line].uiLength = length;
    mDataLines[line].uiText = text;
    mDataLines[line].uiValidIcon = validIcon;
}

std::shared_ptr<QMessageBox> LineTab::check()
{
    int i = 1;
    for(auto & line : mDataLines)
    {
        if(!line.fine)
        {
           auto message = std::make_shared<QMessageBox>();
           message->setText(QString("Нужно выбрать линию № %1").arg(i));
           message->setIcon(QMessageBox::Icon::Critical);
           return message;
        }
        i++;
    }
    return nullptr;
}

void LineTab::updatePower(double power, LineTab::NumberLine typeLine)
{
   mCurrentLine = typeLine;
   mDataLines[mCurrentLine].powerLine = power;
   recountLine("");
}

void LineTab::recountLine()
{
   mCurrentLine = mRegistedObject[sender()];
   getCosAndLength();
   recountLine("");
}

void LineTab::recountLine(QString name)
{
     if(!name.isEmpty())
     {
        mCurrentLine = mRegistedObject[sender()];
        updateWire(name);
     }

     updateDelU();
    updateValidIcon(name);
    emit sendDataLine(static_cast<TabIndices>(mCurrentLine + 8),  mDataLines[mCurrentLine]);
    if(mCurrentLine == LineTab::LINE_WG1 || mCurrentLine == LineTab::LINE_WG2 )
    {
        emit sendDataLine(static_cast<TabIndices>(LINE_1_RESIDENTIAL + 8),  mDataLines[LINE_1_RESIDENTIAL]);
        emit sendDataLine(static_cast<TabIndices>(LINE_2_LIVESTOCK + 8),    mDataLines[LINE_2_LIVESTOCK]);
        emit sendDataLine(static_cast<TabIndices>(LINE_3_INDUSTRIAL + 8),   mDataLines[LINE_3_INDUSTRIAL]);
    }
}

void LineTab::setIndexLine(QString name)
{
    mCurrentLine = mRegistedObject[sender()];
    auto dataWire = mDataWires[name];
    mDataLines[mCurrentLine].dataWire.r0 = dataWire.r;
    mDataLines[mCurrentLine].dataWire.x0 = dataWire.x;
    recountLine("");
}

void LineTab::updateWire(const QString & name)
{
    auto dataWire = mDataWires[name];
    mDataLines[mCurrentLine].dataWire.r = dataWire.r;
    mDataLines[mCurrentLine].dataWire.x = dataWire.x;
    mDataLines[mCurrentLine].dataWire.Ilong = dataWire.Ilong;
    mDataLines[mCurrentLine].dataWire.withNullWire = dataWire.withNullWire;
    if(dataWire.r0 > 0)
    {
        mDataLines[mCurrentLine].dataWire.r0 = dataWire.r0;
        mDataLines[mCurrentLine].dataWire.x0 = dataWire.x0;
    }
    if(mDataLines[mCurrentLine].comBox2)
    {
        mDataLines[mCurrentLine].comBox2->setEnabled(!mDataLines[mCurrentLine].dataWire.withNullWire);
    }
}

void LineTab::updateDelU()
{
    double Pline = mDataLines[mCurrentLine].powerLine;
    double I = Pline * 1000 / (380.0 * SQRT3 * mDataLines[mCurrentLine].cosFi);
    QString str;
    str.setNum(I, 'f', 2);
    auto & dataWire = mDataLines[mCurrentLine].dataWire;
    mDataLines[mCurrentLine].Icount->setText(QString("Iрас = %1 А   <  Iдл.доп = %2 А").arg(str).arg(dataWire.Ilong));
    mDataLines[mCurrentLine].fine = I < dataWire.Ilong;
    mDataLines[mCurrentLine].cond1->setPixmap(mDataLines[mCurrentLine].fine ? QPixmap(":/Resources/Icon/accept2.png") : QPixmap(":/Resources/Icon/warning4.png"));

    auto getQline = [this](NumberLine line) -> double
    {
        double Pline = mDataLines[line].powerLine;
        double cos = mDataLines[line].cosFi;

        double sin = sqrt(1 - pow(cos, 2));
        double tn =  sin / cos;

        double Qline = Pline * tn;
        return Qline;
    };

    auto delta = QChar(0x94, 0x03);
    if(mCurrentLine != LINE_WG1 && mCurrentLine != LINE_WG2)
    {
        double Qline = getQline(mCurrentLine);
        double Pline = mDataLines[mCurrentLine].powerLine;
        double Rline = dataWire.r * (mDataLines[mCurrentLine].length / 1000);
        double Xline1 = dataWire.x * (mDataLines[mCurrentLine].length / 1000);

        double delU = (Pline * Rline + Qline * Xline1) /(pow(0.38,2) * 10);

        auto Rsum = (dataWire.r + dataWire.r0) * mDataLines[mCurrentLine].length / 1000;
        auto Xsum = (dataWire.x + dataWire.x0) * mDataLines[mCurrentLine].length / 1000;
        mDataLines[mCurrentLine].ZloopPhaseZero = (SQRT3 * std::sqrt(pow(Rsum, 2) + pow(Xsum, 2)));

        str.setNum(isnan(delU) ? 0.0 : delU, 'f', 2);
        auto sum = QChar(0xA3, 0x03);
        double showDelUWG = isnan(mDelUWG) ? 0.0 :  mDelUWG;
        mDataLines[mCurrentLine].delU->setText(QString("%1U%2 = %3%+%4% = %5% < %6Uдоп = 10%").arg(delta).arg(sum).arg(QString::number(showDelUWG, 'f', 2)).arg(str).arg(QString::number(showDelUWG + delU, 'f', 2)).arg(delta));
        mDataLines[mCurrentLine].cond3->setText(QString("%1Uрасч = %2%").arg(delta).arg(str));
        mDataLines[mCurrentLine].fine = mDataLines[mCurrentLine].fine && showDelUWG + delU < 10;
        mDataLines[mCurrentLine].cond2->setPixmap(showDelUWG + delU < 10 ? QPixmap(":/Resources/Icon/accept2.png") : QPixmap(":/Resources/Icon/warning4.png"));
    }
    else
    {
         double Pwg1 = mDataLines[LINE_WG1].powerLine;
         auto & dataWireWG1 = mDataLines[LINE_WG1].dataWire;
         double Qwg1 = getQline(LINE_WG1);
         double RlineWG1 = dataWireWG1.r * (mDataLines[LINE_WG1].length / 1000);// > 0 ? dataWireWG1.r * (mDataLines[LINE_WG1].length / 1000) : std::numeric_limits<double>::max();
         double XlineWG1 = dataWireWG1.x * (mDataLines[LINE_WG1].length / 1000);// > 0 ? dataWireWG1.x * (mDataLines[LINE_WG1].length / 1000) : std::numeric_limits<double>::max();

         double Pwg2 = mDataLines[LINE_WG2].powerLine;
         auto & dataWireWG2 = mDataLines[LINE_WG2].dataWire;
         double Qwg2 = getQline(LINE_WG2);
         double RlineWG2 = dataWireWG2.r * (mDataLines[LINE_WG2].length / 1000);// > 0 ? dataWireWG2.r * (mDataLines[LINE_WG2].length / 1000) : std::numeric_limits<double>::max();
         double XlineWG2 = dataWireWG2.x * (mDataLines[LINE_WG2].length / 1000);// > 0 ? dataWireWG2.x * (mDataLines[LINE_WG2].length / 1000) : std::numeric_limits<double>::max();

         double Rcommon = 0.0;
         double Xcommon = 0.0;
         if(RlineWG1 == 0.0)
         {
            Rcommon = RlineWG2;
            Xcommon = XlineWG2;
         }
         else if(RlineWG2 == 0.0)
         {
             Rcommon = RlineWG1;
             Xcommon = XlineWG1;
         }
         else
         {
             Rcommon = ( RlineWG1 *  RlineWG1) / (RlineWG1 + RlineWG2);
             Xcommon = ( XlineWG1  *  XlineWG2) / (XlineWG1 + XlineWG2);
         }

         double Pcommon = Pwg1 + Pwg2;
         double Qcommon = Qwg1 + Qwg2;

         auto val1 = (Pcommon * Rcommon) + (Qcommon * Xcommon);
        auto val2 = (pow(0.38,2) * 10);
        // mDelUWG = (Pcommon * Rcommon) + (Qcommon * Xcommon) /(pow(0.38,2) * 10);
        mDelUWG = val1/val2;
         //mDelUWG = (Pcommon * RlineWG1) + (Qcommon * XlineWG1) /(pow(0.38,2) * 10);
         ui->chemDelU1->setText(QString("%1Uвэу = %2%").arg(delta).arg(QString::number(isnan(mDelUWG) ? 0.0 : mDelUWG, 'f', 2)));
         ui->chemDelU1_2->setText(QString("%1Uвэу = %2%").arg(delta).arg(QString::number(isnan(mDelUWG) ? 0.0 : mDelUWG, 'f', 2)));
         ui->chemDelU1_3->setText(QString("%1Uвэу = %2%").arg(delta).arg(QString::number(isnan(mDelUWG) ? 0.0 : mDelUWG, 'f', 2)));

         double R0lineWG1 = dataWireWG1.r * (mDataLines[LINE_WG1].length / 1000) > 0 ? dataWireWG1.r * (mDataLines[LINE_WG1].length / 1000) : std::numeric_limits<double>::max();
         double X0lineWG1 = dataWireWG1.x * (mDataLines[LINE_WG1].length / 1000) > 0 ? dataWireWG1.x * (mDataLines[LINE_WG1].length / 1000) : std::numeric_limits<double>::max();

         double R0lineWG2 = dataWireWG1.r * (mDataLines[LINE_WG2].length / 1000) > 0 ? dataWireWG1.r * (mDataLines[LINE_WG2].length / 1000) : std::numeric_limits<double>::max();
         double X0lineWG2 = dataWireWG1.x * (mDataLines[LINE_WG2].length / 1000) > 0 ? dataWireWG1.x * (mDataLines[LINE_WG2].length / 1000) : std::numeric_limits<double>::max();

         double ZloopWG1 = sqrt(pow(RlineWG1 + R0lineWG1, 2) + pow(XlineWG1 + X0lineWG1, 2));
         double ZloopWG2 = sqrt(pow(RlineWG2 + R0lineWG2, 2) + pow(XlineWG1 + X0lineWG1, 2));
         double ZloopWGLine = (ZloopWG1 * ZloopWG2) / (ZloopWG1 + ZloopWG2);

         auto tempCurrentLine = mCurrentLine;

         mCurrentLine = LINE_1_RESIDENTIAL;
         mDataLines[mCurrentLine].ZloopWGLine = ZloopWGLine;
         updateDelU();
         mCurrentLine = LINE_2_LIVESTOCK;
         mDataLines[mCurrentLine].ZloopWGLine = ZloopWGLine;
         updateDelU();
         mCurrentLine = LINE_3_INDUSTRIAL;
         mDataLines[mCurrentLine].ZloopWGLine = ZloopWGLine;
         updateDelU();
         mCurrentLine = tempCurrentLine;
    }

}

void LineTab::updateValidIcon(const QString & name)
{
     auto & dataWire = mDataLines[mCurrentLine].dataWire;
    if(dataWire.r > 0 && dataWire.r0 > 0 && mDataLines[mCurrentLine].length > 0)
    {
       if(mDataLines[mCurrentLine].fine)
       {
           mDataLines[mCurrentLine].uiText->setText(QString("Линия включена"));
           mDataLines[mCurrentLine].uiValidIcon->setPixmap(QPixmap(":/Resources/Icon/accept2.png"));
           if(mCurrentLine != LINE_WG1 && mCurrentLine != LINE_WG2)
           {
               emit changeModelValidIcon(static_cast<TabIndices>(mCurrentLine + 8), true);
           }
       }
       else
       {
           mDataLines[mCurrentLine].uiText->setText(QString("Линия не включена"));
           mDataLines[mCurrentLine].uiValidIcon->setPixmap(QPixmap(":/Resources/Icon/warning4.png"));
           if(mCurrentLine != LINE_WG1 && mCurrentLine != LINE_WG2)
           {
               emit changeModelValidIcon(static_cast<TabIndices>(mCurrentLine + 8), false);
           }
       }
    }
    else if(name == "Убрать линию" || dataWire.r == 0.0 || dataWire.r0 == 0.0 || mDataLines[mCurrentLine].length == 0.0 || mDataLines[mCurrentLine].cosFi == 0.0)
    {
        mDataLines[mCurrentLine].uiText->setText(QString("Линия не включена"));
        mDataLines[mCurrentLine].uiValidIcon->setPixmap(QPixmap(":/Resources/Icon/warning4.png"));
        if(mCurrentLine != LINE_WG1 && mCurrentLine != LINE_WG2)
        {
            emit changeModelValidIcon(static_cast<TabIndices>(mCurrentLine + 8), false);
        }
    }
}

QStringList LineTab::getNamesWires()
{
    QStringList list;
    for(auto & wire : mDataWires)
    {

        list.append(wire.name);


    }
    return list;
}

QStringList LineTab::getNamesNullWires()
{
    QStringList list;
    for(auto & wire : mDataWires)
    {
        if(wire.r0 == 0.0)
        {
            list.append(wire.name);
        }
    }
    return list;
}

QStringList LineTab::getNamesWiresWG()
{
    QStringList list;
    for(auto & wire : mDataWires)
    {
        if(wire.onlyWG || wire.Ilong == 0)
        {
            list.append(wire.name);
        }
    }
    return list;
}

void LineTab::getCosAndLength()
{
    double cosValue = mCurrentLine == LINE_WG1 || mCurrentLine == LINE_WG2 ? 0.9 : 1.0;
    if(mDataLines[mCurrentLine].uiCos)
    {
        cosValue = mDataLines[mCurrentLine].uiCos->text().replace(",", ".").toDouble();
        if(cosValue > 1.0)
        {

        }
    }

    mDataLines[mCurrentLine].cosFi  = cosValue;
    mDataLines[mCurrentLine].length = mDataLines[mCurrentLine].uiLength->text().replace(",", ".").toDouble();
}

void LineTab::loadDataFromJson()
{

    QFile listDiesel( ":/Resources/DataWires/ListOfWire.json");
    if( !listDiesel.open( QIODevice::ReadOnly ) )
    {
        //TODO
        return;
    }

    QList<DataWire> listDataWires;

    QByteArray data = listDiesel.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    QJsonObject jsonObject = document.object();

    QJsonValue agentsArrayValue = jsonObject.value("Wires");
    QJsonArray agentsArray = agentsArrayValue.toArray();

    foreach (const QJsonValue & v, agentsArray)
    {
        QFile file(v.toObject().value("path").toString());
        if( !file.open( QIODevice::ReadOnly ) )
        {
           //TODO
            return;
        }

        QByteArray dataWire = file.readAll();
        QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
        QString unicode = defaultTextCodec->toUnicode(dataWire);
        QJsonDocument wireDocument;
        wireDocument = wireDocument.fromJson( unicode.toUtf8() );

        DataWire dw;
        dw.name = wireDocument["name"].toString();
        dw.Ilong = wireDocument["I"].toInt();
        dw.r = wireDocument["r"].toString().toDouble();
        dw.x = wireDocument["x"].toString().toDouble();
        dw.r0 = wireDocument["r0"].toString().toDouble();
        dw.x0 = wireDocument["x0"].toString().toDouble();
        dw.withNullWire = wireDocument["withNullWire"].toBool();
        dw.onlyWG = wireDocument["onlyWG"].toBool();
        mDataWires.insert(dw.name, dw);
     }
}

void LineTab::setTextLable()
{
    ui->cosLIne1Text->setText(QString("Коэффициент нагрузки линии, cos %1").arg(QChar(0x03C6)));
    ui->cosLIne2Text->setText(QString("Коэффициент нагрузки линии, cos %1").arg(QChar(0x03C6)));
    ui->cosLIne3Text->setText(QString("Коэффициент нагрузки линии, cos %1").arg(QChar(0x03C6)));

    auto delta = QChar(0x94, 0x03);
    auto sum = QChar(0xA3, 0x03);
    ui->delU->setText(QString("%1Uрасч = 0% < %2Uдоп = 5%").arg(delta).arg(delta));
    ui->delU_2->setText(QString("%1Uрасч = 0% < %2Uдоп = 5%").arg(delta).arg(delta));
    ui->delU_3->setText(QString("%1Uрасч = 0% < %2Uдоп = 5%").arg(delta).arg(delta));

    ui->chemDelU1->setText(QString("%1Uвэу = 5%").arg(delta));
    ui->chemSumDelU1->setText(QString("%1U%3 < %2Uдоп = 10%").arg(delta).arg(delta).arg(sum));

    ui->chemDelU1_2->setText(QString("%1Uвэу = 5%").arg(delta));
    ui->chemSumDelU1_2->setText(QString("%1U%3 < %2Uдоп = 10%").arg(delta).arg(delta).arg(sum));

    ui->chemDelU1_3->setText(QString("%1Uвэу = 5%").arg(delta));
    ui->chemSumDelU1_3->setText(QString("%1U%3 < %2Uдоп = 10%").arg(delta).arg(delta).arg(sum));

    ui->chemDelU2_1->setText(QString("%1Uрасч = %2% ").arg(delta).arg(0));
    ui->chemDelU2_2->setText(QString("%1Uрасч = %2%").arg(delta).arg(0));
    ui->chemDelU2_3->setText(QString("%1Uрасч = %2% ").arg(delta).arg(0));
}











