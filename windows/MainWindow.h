#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <deque>
#include <QMap>

#include "windows/FuseWindow.h"
#include "tabs/LineTab.h"
#include "tabs/DieselTab.h"
#include "tabs/IndustrialTab.h"
#include "tabs/WindGeneratorTab.h"
#include "tabs/LivestockTab.h"
#include "tabs/DistributionTab.h"
#include "tabs/ResidentialTab.h"
#include "windows/ChargingStationWindow.h"
#include "windows/InfoWindow.h"
#include "windows/LoginWindow.h"
#include "tabs/TabIndex.h"
#include "ui_mainwindow.h"

using namespace  std;
class QIdentityProxyModel;
class QCustomPlot;
class TableCoefficientsPower;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startCalculation();
    void calculatePower();
    double getTotalPowerOfConsumers(int currentMonth, int currentHour);
    void changeValidIcon(TabIndices, bool );
    void stopCalculation();
    void OnInfoWindow();
    void OnLoadOrSaveFile(int type);
private:
    Ui::MainWindow *ui;
    TableCoefficientsPower * mTableCoefficients;
    InfoWindow               mInfoWindow;
    LoginWindow              mLoginWindow;

    std::shared_ptr<QMessageBox> messageBox;
    map<int, vector<shared_ptr<QIdentityProxyModel>>> mkeyProxy;
    QTimer *                        mTimer;
    QMovie *                        mMovieWG1;
    QMovie *                        mMovieWG2;
    bool                            needCheck;
    std::array<QVector<QVariant>,3> mCalculateData;
    QMap<int, QVector<QFrame*>>     mLine;
    ChargingStationWindow           mChargingStationWindow;
    std::array<WGProxyModel::DataWG,3> mDataGenerators;

    struct SpeedRange
    {
        SpeedRange():min(0.0),max(0.0){}
        void reset() { min = 0.0; max = 0.0;}
        double min;
        double max;
    };

    enum DataWG
    {
        WG1,
        WG2,
        Diesel
    };

     SpeedRange mRangeSpeed;
     QLineF line;
     int  mIndexInteration;
     QStack<int> mpreviuosTabs;


    double calculateSpeed(int value);
    void setBeaufortRange();

    template<typename T>
    double getInstalledCapacity(TabIndices type);


    QVector<CellWindow::DataCell> getCellData(TabIndices index);
    void setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData);

    template<typename T>
    double getCurrentPower(TabIndices type, int currentMonth, int currentHour);

    void drawLines(TabIndices index, const char * color, bool reset = false);


public:


    bool checkParams();
    double random(double min, double max);
    int getTotalPowerIndustrialSector();
    double getSumPowerWG();
private slots:
    void loadFromFile();
    void saveInFile();
    void setMinimumWindSpeed(int value);
    void setMaximumWindSpeed(int value);

    void setPreviousTab();
    void updateStackTabs(int index);
    void cheetCheeckButton();
    void chargingStationWindow();
signals:
    void switchElement(int index, CellWindow::DataCell dataCell);
    void setChargingValue(double);

};




Q_DECLARE_METATYPE(LineTab::NumberLine);
Q_DECLARE_METATYPE(TabIndices);

template<typename T> double MainWindow::getInstalledCapacity(TabIndices type)
{

    if(auto tab = qobject_cast<T*>(ui->Tabs->widget(static_cast<int>(type))))
    {
        return tab->getPower();
    }
    return 0.0;
}

template<typename T>
double MainWindow::getCurrentPower(TabIndices type, int currentMonth, int currentHour)
{

    if(auto tab = qobject_cast<T*>(ui->Tabs->widget(static_cast<int>(type))))
    {
        return tab->getPower(currentMonth, currentHour);
    }
    return 0.0;
}


#endif // MAINWINDOW_H
