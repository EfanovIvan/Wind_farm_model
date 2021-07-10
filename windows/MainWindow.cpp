
#include <QMovie>
#include <memory>
#include <QComboBox>
#include <QFileDialog>
#include <streambuf>
#include <sstream>
#include <QValidator>
#include <QTimer>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <memory>
#include <iostream>

#include "windows/MainWindow.h"
#include "ui_mainwindow.h"
#include "models/TableCoefficientsPower.h"
#include "tabs/LineTab.h"
#include "tabs/DieselTab.h"
#include "tabs/IndustrialTab.h"
#include "tabs/WindGeneratorTab.h"
#include "tabs/LivestockTab.h"
#include "tabs/DistributionTab.h"
#include "tabs/ResidentialTab.h"
#include "tabs/ResultTab.h"
#include "utils/CheckSelection.h"
#include "tabs/TabIndex.h"

#define M_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    needCheck(true),
    mTimer(nullptr),
    mIndexInteration(0)
{
        ui->setupUi(this);
        qRegisterMetaType<LineTab::NumberLine>( "LineTab::NumberLine" );
        qRegisterMetaType<TabIndices>( "TabIndices" );

        // click image. Set indices
        ui->WG1->setIndexType(TabIndices::WindInstallations);
        ui->WG2->setIndexType(TabIndices::WindInstallations);
        ui->diesel->setIndexType(TabIndices::Diesel);
        //ui->diesel->setPixmapPath(":/Resources/model/new_disel.jpg");

        ui->line1->setIndexType(TabIndices::Line);
        ui->line2->setIndexType(TabIndices::Line);
        ui->line3->setIndexType(TabIndices::Line);

        ui->LiveSectore->setIndexType(TabIndices::ResidentialSector);
        ui->AdminBuilding->setIndexType(TabIndices::ResidentialSector);

        ui->Birds->setIndexType(TabIndices::LivestockComplex);
        ui->Livestock->setIndexType(TabIndices::LivestockComplex);

        ui->Garage->setIndexType(TabIndices::IndustrialSector);
        ui->Greenhouse->setIndexType(TabIndices::IndustrialSector);
        ui->Shop->setIndexType(TabIndices::IndustrialSector);
        ui->DistributionPoint->setIndexType(TabIndices::DistributionPoint);
        ui->chargingStation->setIndexType(TabIndices::ChargingStation);

        QObject::connect(ui->WG1, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->WG2, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->diesel, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));

        QObject::connect(ui->line1, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->line2, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->line3, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));

        QObject::connect(ui->LiveSectore, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->AdminBuilding, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));

        QObject::connect(ui->Birds, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->Livestock, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));

        QObject::connect(ui->Garage, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->Greenhouse, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->Shop, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        QObject::connect(ui->DistributionPoint, SIGNAL(clicked(int)), ui->Tabs, SLOT(setCurrentIndex(int )));
        ///////////////////////////////////////////////////////////////////////////////////////////


        ui->Tabs->insertTab(Tab::Index::WindInstallations, new WindGeneratorTab, "ВЭУ");
        ui->Tabs->insertTab(Tab::Index::DistributionPoint, new DistributionTab(this), "РУ");
        ui->Tabs->insertTab(Tab::Index::IndustrialSector, new IndustrialTab(this), "Промышленный сектор");
        ui->Tabs->insertTab(Tab::Index::ResidentialSector, new ResidentialTab(this), "Жилой сектор");
        ui->Tabs->insertTab(Tab::Index::LivestockComplex, new Livestock(this), "Животноводческий комплекс");
        ui->Tabs->insertTab(Tab::Index::Diesel, new DieselTab(this), "ДЭГ");
        ui->Tabs->insertTab(Tab::Index::Line, new LineTab(this), "Линии");
        ui->Tabs->insertTab(Tab::Index::Result, new ResultTab(this), "Результаты");



         mTableCoefficients = new TableCoefficientsPower(this);
         if(auto tab = qobject_cast<IndustrialTab*>(ui->Tabs->widget(Tab::Index::IndustrialSector)))
         {
             tab->setModel(mTableCoefficients);
         }

         if(auto tab = qobject_cast<ResidentialTab*>(ui->Tabs->widget(Tab::Index::ResidentialSector)))
         {
             tab->setModel(mTableCoefficients);
         }

         if(auto tab = qobject_cast<Livestock*>(ui->Tabs->widget(Tab::Index::LivestockComplex)))
         {
             tab->setModel(mTableCoefficients);
         }

         //Buttons initialization
         QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startCalculation()));
         ///////

         QObject::connect(ui->sliderMin, SIGNAL(valueChanged(int)), this, SLOT(setMinimumWindSpeed(int)));
         QObject::connect(ui->sliderMax, SIGNAL(valueChanged(int)), this, SLOT(setMaximumWindSpeed(int)));

        //QObject::connect(ui->SelectDiesel->model(), SIGNAL(changeWarningIcon(QPixmap )), ui->warningDiesel, SLOT(setPixmap(QPixmap)));
        QObject::connect(ui->Tabs->widget(Tab::Index::IndustrialSector), SIGNAL(changePower(double, LineTab::NumberLine)), ui->Tabs->widget(Tab::Index::Line), SLOT(updatePower(double, LineTab::NumberLine)));
        QObject::connect(ui->Tabs->widget(Tab::Index::LivestockComplex), SIGNAL(changePower(double, LineTab::NumberLine)), ui->Tabs->widget(Tab::Index::Line), SLOT(updatePower(double, LineTab::NumberLine)));
        QObject::connect(ui->Tabs->widget(Tab::Index::ResidentialSector), SIGNAL(changePower(double, LineTab::NumberLine)), ui->Tabs->widget(Tab::Index::Line), SLOT(updatePower(double, LineTab::NumberLine)));
        QObject::connect(ui->Tabs->widget(Tab::Index::WindInstallations), SIGNAL(changePower(double, LineTab::NumberLine)), ui->Tabs->widget(Tab::Index::Line), SLOT(updatePower(double, LineTab::NumberLine)));

        QObject::connect(ui->Tabs->widget(Tab::Index::IndustrialSector), SIGNAL(updatePower(TabIndices, double)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(setPowerLine(TabIndices, double)));
        QObject::connect(ui->Tabs->widget(Tab::Index::LivestockComplex), SIGNAL(updatePower(TabIndices, double)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(setPowerLine(TabIndices, double)));
        QObject::connect(ui->Tabs->widget(Tab::Index::ResidentialSector), SIGNAL(updatePower(TabIndices, double)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(setPowerLine(TabIndices, double)));

        QObject::connect(ui->Tabs->widget(Tab::Index::WindInstallations), SIGNAL(changeModelValidIcon(TabIndices, bool)), this, SLOT(changeValidIcon(TabIndices, bool)));
        QObject::connect(ui->Tabs->widget(Tab::Index::WindInstallations), SIGNAL(changeModelValidIcon(TabIndices, bool)), this, SLOT(changeValidIcon(TabIndices, bool)));
        QObject::connect(ui->Tabs->widget(Tab::Index::Diesel), SIGNAL(changeModelValidIcon(TabIndices, bool)), this, SLOT(changeValidIcon(TabIndices, bool)));
        QObject::connect(ui->Tabs->widget(Tab::Index::Line), SIGNAL(changeModelValidIcon(TabIndices, bool)), this, SLOT(changeValidIcon(TabIndices, bool)));

        QObject::connect(ui->Tabs->widget(Tab::Index::Line), SIGNAL(sendDataLine(TabIndices, LineTab::DataLine)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(setDataLine(TabIndices, LineTab::DataLine)));


        QObject::connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopCalculation()));
        QObject::connect(ui->cheetCheckButtonOff, SIGNAL(clicked()), this, SLOT(cheetCheeckButton()));
        QObject::connect(ui->cheetCheckButtonOn, SIGNAL(clicked()), this, SLOT(cheetCheeckButton()));
        ui->cheetCheckButtonOn->setEnabled(false);
        QObject::connect(ui->chargingStation, SIGNAL(clicked(int)), this, SLOT(chargingStationWindow( )));

        mMovieWG1 = new QMovie( ":/Resources/model/newWG.gif");
        mMovieWG2 = new QMovie( ":/Resources/model/newWG.gif");
        ui->WG1->setMovie(mMovieWG1);
        ui->WG2->setMovie(mMovieWG2);
        mMovieWG1->start();
        mMovieWG2->start();
        mMovieWG1->setSpeed(0);
        mMovieWG2->setSpeed(0);

        QObject::connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
        QObject::connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInFile()));

        QObject::connect(this, SIGNAL(clicked()), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(loadFromFile()));
        QObject::connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(OnInfoWindow()));
        QObject::connect(&mLoginWindow, SIGNAL(closeAuthorizationWindow(int)), this, SLOT(OnLoadOrSaveFile(int)));


        mLine.insert(static_cast<int>(TabIndices::WG1), {ui->wg1_line1, ui->wg1_line2, ui->wg1_line3});
        mLine.insert(static_cast<int>(TabIndices::WG2), {ui->wg2_line1});
        mLine.insert(static_cast<int>(TabIndices::Diesel), {ui->diesel_line1, ui->diesel_line2, ui->diesel_line3});
        mLine.insert(static_cast<int>(TabIndices::LivestockComplex), {ui->LineOUT2_1, ui->LineOUT2_2, ui->LineOUT2_3, ui->LineOUT2_4});
        mLine.insert(static_cast<int>(TabIndices::IndustrialSector), {ui->LineOUT3_1, ui->LineOUT3_2, ui->LineOUT3_3, ui->LineOUT3_4, ui->LineOUT3_5});
        mLine.insert(static_cast<int>(TabIndices::ResidentialSector), {ui->LineOUT1_1, ui->LineOUT1_2, ui->LineOUT1_3, ui->LineOUT1_4});
        mLine.insert(static_cast<int>(TabIndices::Line1), {ui->LineIN1_1, ui->LineIN1_2, ui->LineIN1_3});
        mLine.insert(static_cast<int>(TabIndices::Line2), {ui->LineIN2_1});
        mLine.insert(static_cast<int>(TabIndices::Line3), {ui->LineIN3_1, ui->LineIN3_2, ui->LineIN3_3});
        mLine.insert(static_cast<int>(TabIndices::ChargingStation), {ui->gs_line1/*, ui->gs_line2, ui->gs_line3*/});


        QObject::connect(ui->Tabs, SIGNAL(currentChanged(int)), this, SLOT(updateStackTabs(int  )));
        mpreviuosTabs.push(ui->Tabs->currentIndex());
        QObject::connect(ui->Tabs->widget(Tab::Index::WindInstallations), SIGNAL(changePower(double)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(changePowerWG(double)));
        QObject::connect(this, SIGNAL(setChargingValue(double)), ui->Tabs->widget(Tab::Index::DistributionPoint), SLOT(setChargingValue(double)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(mTimer != nullptr)
    {
        delete mTimer;
    }
}

void MainWindow::startCalculation()
{
    if(needCheck)
    {
        if(!checkParams())return;
    }

    mTimer = new QTimer();
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(calculatePower()));
    srand (static_cast <unsigned> (time(nullptr)));
    mTimer->setInterval(15000);
    mTimer->start();


    if(auto tab = qobject_cast<DieselTab*>(ui->Tabs->widget(Tab::Index::Diesel)))
    {
        mDataGenerators[DataWG::Diesel] = tab->getDataDiesel();
    }

    if(auto tab = qobject_cast<WindGeneratorTab*>(ui->Tabs->widget(Tab::Index::WindInstallations)))
    {
        mDataGenerators[DataWG::WG1] = tab->getDataWG1();
        mDataGenerators[DataWG::WG2] = tab->getDataWG2();
    }

    calculatePower();
}

void MainWindow::calculatePower()
{
    if(mIndexInteration == 12)
    {
        mIndexInteration = 0;
        mTimer->stop();
        mMovieWG1->stop();
        mMovieWG2->stop();
        if(auto tab = qobject_cast<ResultTab*>(ui->Tabs->widget(Tab::Index::Result)))
        {
            tab->setCalculationData(mCalculateData);
        }
        return;
    }

    ui->currentMonthLCD->display(mIndexInteration + 1);
    setBeaufortRange();

    WGProxyModel::DataWG wg1 =  mDataGenerators[DataWG::WG1];
    WGProxyModel::DataWG wg2 =  mDataGenerators[DataWG::WG2];
    WGProxyModel::DataWG diesel = mDataGenerators[DataWG::Diesel];

    double generatedPowerByWG = 0.0;
    double generatedPowerByDiesel = 0.0;
    double storedInBattery = 0.0;

    for(int hour = 0; hour < 24; hour++ )
    {
        auto dieselCell = getCellData(TabIndices::Diesel);
        if(dieselCell[0].automatic )
        {
            dieselCell[0].onContactor = false;
            this->drawLines(TabIndices::Diesel, "color: rgb(0,0,0)");
            setCellData(TabIndices::Diesel, dieselCell);
        }

        double powerBalance = diesel.isSelected ? diesel.nomPower / 1000 : 0.0;

        double powerConsumers = getTotalPowerOfConsumers(mIndexInteration, hour);
        ui->consumerPower->display(powerConsumers);


        double V = random(mRangeSpeed.min, mRangeSpeed.max);
        ui->windSpeedLCD->display(V);

        auto getGeneratedPower = [this, V](WGProxyModel::DataWG wg, QMovie * movieWG, TabIndices index) -> double
        {
             auto cell = getCellData(index);
            if( wg.isSelected && V >= wg.speedRange.first && V <= wg.speedRange.second )
            {
                double P = wg.speedPowerDependence[std::ceil(V)];

                movieWG->start();
                movieWG->setSpeed(V * 30);
                if(cell.front().automatic || (cell.front().onContactor && cell.front().onDisconnector))
                {
                    cell.front().onContactor = true;
                    setCellData(index, cell);
                   this->drawLines(index, "color: rgb(0,255,0)");
                   return P > wg.nomPower ? wg.nomPower : P;
                }
            }
            else
            {
                if(cell.front().automatic)
                {
                    cell.front().onContactor = false;
                    setCellData(index, cell);
                }
                movieWG->stop();
                this->drawLines(index, "color: rgb(0,0,0)");
            }
            return 0.0;
        };

        double powerGenerated1 = getGeneratedPower(wg1, mMovieWG1, TabIndices::WG1);
        double powerGenerated2 = getGeneratedPower(wg2, mMovieWG2, TabIndices::WG2);
        double generatedCurrentHoure = (powerGenerated1 + powerGenerated2) / 1000;
        ui->lcdNumber_6->display((generatedCurrentHoure * 100) / ((wg1.nomPower + wg2.nomPower)/ 1000));
        ui->lcdNumber_5->display(generatedCurrentHoure);

        auto switchConsumer = [this, &generatedCurrentHoure, &generatedPowerByWG, &generatedPowerByDiesel, hour, &powerBalance](double power, TabIndices index, TabIndices lineIndex, double currentPower)
        {
            auto cells = getCellData(index);
            if(power != 0.0)
            {
                if(generatedCurrentHoure > power)
                {
                     if(cells[0].automatic || (cells[1].onContactor && cells[1].onDisconnector))
                     {
                         cells[0].onContactor = false;
                         cells[1].onContactor = true;
                         generatedPowerByWG += currentPower;
                         generatedCurrentHoure -= currentPower;
                         setCellData(index, cells);
                         this->drawLines(index, "color: rgb(0,255,0)");
                         this->drawLines(lineIndex, "color: rgb(0,255,0)");
                     }
                }
                else if(powerBalance > power)
                {
                    auto dieselCell = getCellData(TabIndices::Diesel);
                    if(dieselCell[0].automatic || (dieselCell[1].onContactor && dieselCell[1].onDisconnector))
                    {
                        cells[0].onContactor = true;
                        cells[1].onContactor = false;
                        dieselCell[0].onContactor = true;
                        generatedPowerByDiesel += currentPower;
                        //powerBalance -= power;
                        powerBalance -= currentPower;
                        this->drawLines(TabIndices::Diesel, "color: rgb(255,0,0)");
                        this->drawLines(lineIndex, "color: rgb(255,0,0)");
                        this->drawLines(index, "color: rgb(255,0,0)");
                        setCellData(TabIndices::Diesel, dieselCell);
                        setCellData(index, cells);
                    }
                    else
                    {
                        this->drawLines(TabIndices::Diesel, "color: rgb(0,0,0)");
                        this->drawLines(index, "color: rgb(0,0,0)");
                        this->drawLines(lineIndex, "color: rgb(0,0,0)");
                    }
                }
                else
                {
                    if(cells[0].automatic)
                    {
                        cells[0].onContactor = false;
                        cells[1].onContactor = false;
                        setCellData(index, cells);
                    }

                    this->drawLines(index, "color: rgb(0,0,0)");
                    this->drawLines(lineIndex, "color: rgb(0,0,0)");
                }
            }
        };

      double installedPower = getInstalledCapacity<Livestock>(TabIndices::LivestockComplex);
      switchConsumer(installedPower, TabIndices::LivestockComplex, TabIndices::Line2, getCurrentPower<Livestock>(TabIndices::LivestockComplex, mIndexInteration, hour));

      installedPower = getInstalledCapacity<IndustrialTab>(TabIndices::IndustrialSector);
      switchConsumer(installedPower, TabIndices::IndustrialSector, TabIndices::Line3, getCurrentPower<IndustrialTab>(TabIndices::IndustrialSector, mIndexInteration, hour));

      installedPower =  getInstalledCapacity<ResidentialTab>(TabIndices::ResidentialSector);
      switchConsumer(installedPower, TabIndices::ResidentialSector, TabIndices::Line1, getCurrentPower<ResidentialTab>(TabIndices::ResidentialSector, mIndexInteration, hour));

       auto cell = getCellData(TabIndices::ChargingStation);
       if(generatedCurrentHoure > 0)
       {
            if(cell[0].automatic || (cell[0].onContactor && cell[0].onDisconnector))
            {
                cell[0].onContactor = true;
                drawLines(TabIndices::ChargingStation, "color: rgb(0,255,0)");
                mChargingStationWindow.setChargingValue(generatedCurrentHoure);
                emit setChargingValue(generatedCurrentHoure);
                setCellData(TabIndices::ChargingStation, cell);
                storedInBattery += generatedCurrentHoure;
            }
       }
       else
       {
           if(cell[0].automatic)
           {
               cell[0].onContactor = false;
               setCellData(TabIndices::ChargingStation, cell);
           }
           drawLines(TabIndices::ChargingStation, "color: rgb(0,0,0)");
           mChargingStationWindow.setChargingValue(0.0);
           emit setChargingValue(0.0);
       }

       ui->lcdNumber_7->display((diesel.nomPower / 1000)  - powerBalance);
       ui->lcdNumber_8->display((((diesel.nomPower / 1000)   - powerBalance) * 100 ) / (diesel.nomPower / 1000 ));

    }
    mCalculateData[0].push_back(generatedPowerByWG / 24); //the number of hour in a day
    mCalculateData[1].push_back(generatedPowerByDiesel / 24);
    mCalculateData[2].push_back(storedInBattery / 24);
    mIndexInteration++;
}

double MainWindow::getTotalPowerOfConsumers(int currentMonth, int currentHour)
{
    double powerConsumers = 0.0;
    if(auto tab = qobject_cast<IndustrialTab*>(ui->Tabs->widget(static_cast<int>(TabIndices::IndustrialSector))))
    {
        powerConsumers += tab->getPower(currentMonth, currentHour);
    }

    if(auto tab = qobject_cast<ResidentialTab*>(ui->Tabs->widget(static_cast<int>(TabIndices::ResidentialSector))))
    {
        powerConsumers += tab->getPower(currentMonth, currentHour);
    }

    if(auto tab = qobject_cast<Livestock*>(ui->Tabs->widget(static_cast<int>(TabIndices::LivestockComplex))))
    {
        powerConsumers += tab->getPower(currentMonth, currentHour);
    }
    return powerConsumers;
}


QVector<CellWindow::DataCell> MainWindow::getCellData(TabIndices index)
{
    if(auto tab = qobject_cast<DistributionTab*>(ui->Tabs->widget(static_cast<int>(TabIndices::DistributionPoint))))
    {
        return tab->getCellData(index);
    }
    return QVector<CellWindow::DataCell>();
}

void MainWindow::setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData)
{
    if(auto tab = qobject_cast<DistributionTab*>(ui->Tabs->widget(static_cast<int>(TabIndices::DistributionPoint))))
    {
        return tab->setCellData(index, cellData);
    }
}

void MainWindow::changeValidIcon(TabIndices type, bool fine)
{
    if(type == TabIndices::WG1)
    {
        ui->warningWG1->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
    else if(type == TabIndices::WG2)
    {
        ui->warningWG2->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
    else if(type == TabIndices::Diesel)
    {
        ui->warningDiesel->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
    else if(type == TabIndices::Line1)
    {
        ui->checkIconLine1->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
    else if(type == TabIndices::Line2)
    {
        ui->checkIconLine2->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
    else if(type == TabIndices::Line3)
    {
        ui->checkIconLine3->setPixmap(!fine ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    }
}

void MainWindow::stopCalculation()
{
    mIndexInteration = 0;
    if(mTimer != nullptr)
    {
        mTimer->stop();
    }

    if(mMovieWG1->isValid() && mMovieWG2->isValid())
    {
        mMovieWG1->stop();
        mMovieWG2->stop();
    }

    ui->lcdNumber_6->display(0);
    ui->lcdNumber_5->display(0);
    ui->windSpeedLCD->display(0);
   // ui->currentMonthLCD->display(0);
    ui->sliderMin->setEnabled(true);
    ui->sliderMax->setEnabled(true);
    if(auto tab = qobject_cast<ResultTab*>(ui->Tabs->widget(Tab::Index::Result)))
    {
        tab->clearData();
    }
    drawLines(TabIndices::WG1, "color: rgb(0,0,0)", true);
}

void MainWindow::OnInfoWindow()
{
    mInfoWindow.show();
}

void MainWindow::OnLoadOrSaveFile(int type)
{
    if(mLoginWindow.getAuthorization() && type == LoginWindow::TypeAuthorization::LOAD)
    {
        QString path = QFileDialog::getOpenFileName(0,QObject::tr("Укажите файл для загрузки"),QDir::homePath(), QObject::tr("Файл (*.json)"));
        if(!path.isEmpty())
        {
            for(int i = 1; i < ui->Tabs->count(); i++)
            {
                if(auto obj = dynamic_cast<LoadDataFromFile*>(ui->Tabs->widget(i)))
                {
                    obj->loadDataFromFile(path);
                }
            }
        }
    }
    else if(mLoginWindow.getAuthorization() && type == LoginWindow::TypeAuthorization::SAVE)
    {
       QString path = QFileDialog::getExistingDirectory(0,QObject::tr("Укажите директорую куда сохранить файл"), QDir::homePath());
       if(!path.isEmpty())
       {
           QFile file(path + "/lab.json");
           file.open(QIODevice::WriteOnly);
           QJsonDocument document;
           document = QJsonDocument().fromJson(file.readAll());
           QJsonObject GBjsonObj;
           for(int i = 1; i < ui->Tabs->count(); i++)
           {
               if(auto obj = dynamic_cast<LoadDataFromFile*>(ui->Tabs->widget(i)))
               {
                   obj->saveInFile(GBjsonObj);
               }
           }
           document.setObject(GBjsonObj);
           file.write(document.toJson());
           file.close();
       }
    }
}

void MainWindow::setMinimumWindSpeed(int value)
{
    auto minSpeed = calculateSpeed(value);
    if(minSpeed > mRangeSpeed.max)
    {
        minSpeed = mRangeSpeed.max;
        ui->sliderMin->setValue(minSpeed * 10);
        messageBox.reset();
        messageBox = std::make_shared<QMessageBox>();
        messageBox->setText(QString("Нижняя граница не может превышать верхнюю"));
        messageBox->show();
    }
    mRangeSpeed.min = minSpeed;
     QString str;
    ui->speedLow->setText(str.setNum(mRangeSpeed.min, 'f', 1) + QString(" м/с"));

}

void MainWindow::setMaximumWindSpeed(int value)
{
    auto maxSpeed = calculateSpeed(value);
    if(maxSpeed < mRangeSpeed.min)
    {
        maxSpeed = mRangeSpeed.min;
        ui->sliderMax->setValue(maxSpeed * 10);
        messageBox.reset();
        messageBox = std::make_shared<QMessageBox>();
        messageBox->setText(QString("Верхняя граница не может быть меньше нижней"));
        messageBox->show();
    }

    mRangeSpeed.max = maxSpeed;
    QString str;
    ui->speedHigh->setText(str.setNum(mRangeSpeed.max, 'f', 1) + QString(" м/с"));
}

void MainWindow::setPreviousTab()
{
    int index = mpreviuosTabs.pop();
    if(index != 0)
    {
        index = mpreviuosTabs.top();
        if(index == 0)
        {
            //ui->previousButton->setEnabled(false);
        }
        ui->Tabs->setCurrentIndex(index);
        mpreviuosTabs.front();
    }
    else
    {
       ui->Tabs->setCurrentIndex(index);;
       //ui->previousButton->setEnabled(false);
    }

}

void MainWindow::updateStackTabs(int index)
{
    if(index != mpreviuosTabs.top())
    {
        if(index == 0)
        {
            mpreviuosTabs.clear();
            //ui->previousButton->setEnabled(false);
        }

        mpreviuosTabs.push(index);
      //  ui->previousButton->setEnabled(true);
    }
    else if(index == 0)
    {
        mpreviuosTabs.clear();
    }

}

void MainWindow::cheetCheeckButton()
{
    needCheck = !needCheck;
    ui->cheetCheckButtonOff->setEnabled(needCheck);
    ui->cheetCheckButtonOn->setEnabled(!needCheck);
}

void MainWindow::chargingStationWindow()
{
    mChargingStationWindow.show();
}

double MainWindow::calculateSpeed(int value)
{
    double result = 0.0;
    result += value / 100;
    if(result > 0)
    {
        result *= 10.0;
        value %=  100;
    }
    result += value / 10;
    double f = value % 10 ;
    f /= 10.0;
    result += f;
    return result;
}

void MainWindow::setBeaufortRange()
{
    if(auto tab = qobject_cast<ResultTab*>(ui->Tabs->widget(Tab::Index::Result)))
    {
       auto range = tab->getBeaufortRange(mIndexInteration);
       mRangeSpeed.reset();
       ui->sliderMax->setValue(range.second * 10);
       ui->sliderMin->setValue(range.first * 10);
       setMaximumWindSpeed(range.second * 10);
       setMinimumWindSpeed(range.first * 10);
       ui->sliderMin->setEnabled(false);
       ui->sliderMax->setEnabled(false);
    }
}

void MainWindow::loadFromFile()
{
    mLoginWindow.setAuthorizationType(LoginWindow::TypeAuthorization::LOAD);
    mLoginWindow.show();
}

void MainWindow::saveInFile()
{
    mLoginWindow.setAuthorizationType(LoginWindow::TypeAuthorization::SAVE);
    mLoginWindow.show();
}

bool MainWindow::checkParams()
{
    for(int i = 1; i < ui->Tabs->count(); i++)
    {
        if(auto obj = dynamic_cast<CheckSelection*>(ui->Tabs->widget(i)))
        {
            messageBox = obj->check();
            if(messageBox)
            {
                int ret = messageBox->exec();
                switch (ret)
                {
                    case QMessageBox::Ok:
                    {
                        ui->Tabs->setCurrentIndex(i);
                    }
                }
                return false;
            }
        }
    }
    return true;
}

double MainWindow::random(double min, double max)
{
    return min + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(max-min)));
}

void MainWindow::drawLines(TabIndices index, const char * color, bool reset )
{
    if(reset)
    {
        for(auto it = mLine.begin(); it != mLine.end(); ++it)
        {
            for(auto & line : *it)
            {
                line->setStyleSheet(color);
            }
        }
        return;
    }

    for(auto & line : mLine[static_cast<int>(index)])
    {
        line->setStyleSheet(color);
    }
}

double MainWindow::getSumPowerWG()
{
    if(auto tab = qobject_cast<WindGeneratorTab*>(ui->Tabs->widget(Tab::Index::WindInstallations)))
    {
        auto wg1 = tab->getDataWG1();
        auto wg2 = tab->getDataWG2();
        return wg1.nomPower + wg2.nomPower;
    }
    return 0;
}








