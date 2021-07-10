#include "DistributionTab.h"
#include "ui_distribution.h"

DistributionTab::DistributionTab(QWidget *parent) : QWidget(parent),ui(new Ui::Distribution)
{
    ui->setupUi(this);

    ui->Dis3->setIndexType(TabIndices::Line3);
    ui->Melt4->setIndexType(TabIndices::Line3);
    QObject::connect(ui->Melt3, SIGNAL(clicked(int )), this, SLOT(opentFuseWindow(int)));
    QObject::connect(ui->Melt4, SIGNAL(clicked(int )), this, SLOT(opentFuseWindow(int)));

    ui->Melt5->setIndexType(TabIndices::Line1);
    ui->Melt6->setIndexType(TabIndices::Line1);
    QObject::connect(ui->Melt5, SIGNAL(clicked(int )), this, SLOT(opentFuseWindow(int)));
    QObject::connect(ui->Melt6, SIGNAL(clicked(int )), this, SLOT(opentFuseWindow(int)));

    ui->Melt7->setIndexType(TabIndices::Line2);
    ui->Melt8->setIndexType(TabIndices::Line2);
    QObject::connect(ui->Melt7, SIGNAL(clicked(int )), this, SLOT(opentFuseWindow(int)));
    QObject::connect(ui->Melt8, SIGNAL(clicked(int)), this, SLOT(opentFuseWindow(int)));


    ui->Dis2->setIndexType(TabIndices::WG2);
    ui->Con2->setIndexType(TabIndices::WG2);
    ui->Dis1->setIndexType(TabIndices::WG1);
    ui->Con1->setIndexType(TabIndices::WG1);
    ui->Dis10->setIndexType(TabIndices::Diesel);
    ui->Dis9->setIndexType(TabIndices::ChargingStation);
    ui->Con10->setIndexType(TabIndices::Diesel);
    ui->Con9->setIndexType(TabIndices::ChargingStation);

    ui->cell1->setIndexType(TabIndices::WG1);
    ui->cell2->setIndexType(TabIndices::WG2);
    ui->cell3->setIndexType(TabIndices::IndustrialSector);
    ui->cell4->setIndexType(TabIndices::ResidentialSector);
    ui->cell5->setIndexType(TabIndices::LivestockComplex);
    ui->cell6->setIndexType(TabIndices::ChargingStation);
    ui->cell7->setIndexType(TabIndices::Diesel);


    QObject::connect(ui->Dis2, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Con2, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Dis1, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Con1, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    ////////////////diesel and charging station
    QObject::connect(ui->Dis9, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Dis10, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Con9, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->Con10, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));

/////////////////////////////////////////////////////////
    QObject::connect(ui->cell1, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->cell2, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->cell3, SIGNAL(clicked(int)), this, SLOT(opentDoubleCellWindow(int )));
    QObject::connect(ui->cell4, SIGNAL(clicked(int)), this, SLOT(opentDoubleCellWindow(int )));
    QObject::connect(ui->cell5, SIGNAL(clicked(int)), this, SLOT(opentDoubleCellWindow(int )));
    QObject::connect(ui->cell6, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
    QObject::connect(ui->cell7, SIGNAL(clicked(int)), this, SLOT(opentCellWindow(int )));
/////////////////////////////line3/////////////////////////////////////////////////////////////////////////
    ui->Dis3->setIndexType(TabIndices::IndustrialSector);
    ui->Dis4->setIndexType(TabIndices::IndustrialSector);
    ui->Con3->setIndexType(TabIndices::IndustrialSector);
    ui->Con4->setIndexType(TabIndices::IndustrialSector);
    QObject::connect(ui->Dis3, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Dis4, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con3, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con4, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));

    ui->Dis11->setIndexType(TabIndices::Shop);
    ui->Dis12->setIndexType(TabIndices::Garage);
    ui->Dis13->setIndexType(TabIndices::Greenhouse);

    QObject::connect(ui->Dis11, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));
    QObject::connect(ui->Dis12, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));
    QObject::connect(ui->Dis13, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));

/////////////////////////// line1
    ui->Dis5->setIndexType(TabIndices::ResidentialSector);
    ui->Dis6->setIndexType(TabIndices::ResidentialSector);
    ui->Con5->setIndexType(TabIndices::ResidentialSector);
    ui->Con6->setIndexType(TabIndices::ResidentialSector);
    QObject::connect(ui->Dis5, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Dis6, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con5, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con6, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));

    ui->Dis14->setIndexType(TabIndices::Building);
    QObject::connect(ui->Dis14, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));

///////////////////////////////line2

    ui->Dis7->setIndexType(TabIndices::LivestockComplex);
    ui->Dis8->setIndexType(TabIndices::LivestockComplex);
    ui->Con7->setIndexType(TabIndices::LivestockComplex);
    ui->Con8->setIndexType(TabIndices::LivestockComplex);
    QObject::connect(ui->Dis7, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Dis8, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con7, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->Con8, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));

    ui->Dis15->setIndexType(TabIndices::Farm);
    QObject::connect(ui->Dis15, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));
    ui->Dis16->setIndexType(TabIndices::BirdHouse);
    QObject::connect(ui->Dis16, SIGNAL(clicked(int)), this, SLOT(opentCellWindowWithoutContactor(int )));
/////////////////////////////////////////////////////////////////////////////////////////////////////////

    ui->P5->setIndexType(TabIndices::ResidentialSector);
    ui->P6->setIndexType(TabIndices::ResidentialSector);
    QObject::connect(ui->P5, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->P6, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));

    ui->P7->setIndexType(TabIndices::LivestockComplex);
    ui->P8->setIndexType(TabIndices::LivestockComplex);
    QObject::connect(ui->P7, SIGNAL(clicked(int )), this, SLOT(opentDoubleCellWindow(int)));
    QObject::connect(ui->P8, SIGNAL(clicked(int)), this, SLOT(opentDoubleCellWindow(int)));


    QObject::connect(&mCellWindow, SIGNAL(changStateDisconnector(int, CellWindow::DataCell)), this, SLOT(changStateDisconnector(int, CellWindow::DataCell)));
    QObject::connect(&mDoubleCellWindow, SIGNAL(changStateDisconnector(int, CellWindow::DataCell)), this, SLOT(changStateDisconnector(int, CellWindow::DataCell)));
    QObject::connect(&mCellWindowWithoutContactor, SIGNAL(changStateDisconnector(int, CellWindow::DataCell)), this, SLOT(changStateDisconnector(int, CellWindow::DataCell)));

    mRegObject[TabIndices::WG2] = qMakePair(ui->disconnector2, ui->contactor2);
    mRegObject[TabIndices::WG1] = qMakePair(ui->disconnector1, ui->contactor1);
    mRegObject[TabIndices::Diesel] = qMakePair(ui->disconnector3, ui->contactor3);
    mRegObject[TabIndices::ChargingStation] = qMakePair(ui->disconnector4, ui->contactor4);
    mRegObject[TabIndices::IndustrialSector] = qMakePair(ui->P3, ui->KM3);
    mRegObject[TabIndices::IndustrialSector_2] = qMakePair(ui->P4, ui->KM4);
    mRegObject[TabIndices::ResidentialSector] = qMakePair(ui->P5, ui->KM5);
    mRegObject[TabIndices::ResidentialSector_2] = qMakePair(ui->P6, ui->KM6);
    mRegObject[TabIndices::LivestockComplex] = qMakePair(ui->P7, ui->KM7);
    mRegObject[TabIndices::LivestockComplex_2] = qMakePair(ui->P8, ui->KM8);

    mRegObject[TabIndices::Shop] = qMakePair(ui->P11, nullptr);
    mRegObject[TabIndices::Garage] = qMakePair(ui->P12, nullptr);
    mRegObject[TabIndices::Greenhouse] = qMakePair(ui->P13, nullptr);
    mRegObject[TabIndices::Farm] = qMakePair(ui->P15, nullptr);
    mRegObject[TabIndices::BirdHouse] = qMakePair(ui->P16, nullptr);
    mRegObject[TabIndices::Building] = qMakePair(ui->P14, nullptr);
   // mRegObject[TabIndices::AdminBuilding] = qMakePair(ui->P8, ui->KM8);
    mCellWindow.updateAllElement();
    mDoubleCellWindow.updateAllElement();
    //mCellWindowWithoutContactor.updateAllElement();
    mFuseWindow.setParent(this);

}

DistributionTab::~DistributionTab()
{
    delete ui;
}

std::shared_ptr<QMessageBox> DistributionTab::check()
{

    if(mFuseWindow.checkFuse())
    {
        auto message = std::make_shared<QMessageBox>();
        message->setText(QString("Для работы схемы нужно выбрать все предохранители"));
        message->setIcon(QMessageBox::Icon::Critical);
        return message;
    }
    return nullptr;
}

void DistributionTab::opentFuseWindow(int index)
{

    mFuseWindow.setLineNumber(index);
    mFuseWindow.show();
}

void DistributionTab::opentCellWindow(int index)
{
    mCellWindow.setIndexCell(index);
    mCellWindow.show();
}

void DistributionTab::opentDoubleCellWindow(int index)
{
    mDoubleCellWindow.setIndexCell(index);
    mDoubleCellWindow.show();
}

void DistributionTab::opentCellWindowWithoutContactor(int index)
{
    mCellWindowWithoutContactor.setIndexCell(index);
    mCellWindowWithoutContactor.show();
}

QVector<CellWindow::DataCell> DistributionTab::getCellData(TabIndices index)
{
    if(index == TabIndices::WG2 || index == TabIndices::WG1 || index == TabIndices::Diesel || index == TabIndices::ChargingStation)
    {
        return mCellWindow.getCellData(index);
    }
    else if(index > TabIndices::Shop)
    {
        return mCellWindowWithoutContactor.getCellData(index);
    }
    else
    {
        return mDoubleCellWindow.getCellData(index);
    }
}

void DistributionTab::setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData)
{
    if(index == TabIndices::WG2 || index == TabIndices::WG1 || index == TabIndices::Diesel || index == TabIndices::ChargingStation)
    {
        return mCellWindow.setCellData(index, cellData);
    }
    else if(index > TabIndices::Shop)
    {
        mCellWindowWithoutContactor.setCellData(index, cellData);
    }
    else
    {
        mDoubleCellWindow.setCellData(index, cellData);
    }
}

void DistributionTab::setPowerLine(TabIndices line, double power)
{
    mFuseWindow.setPower(line, power);
}

void DistributionTab::setDataLine(TabIndices line, LineTab::DataLine dataLine)
{
    mFuseWindow.setParamLine(line, dataLine);
}

void DistributionTab::changePowerWG(double sumPower)
{
    mFuseWindow.setSumPowerWG(sumPower);
}

void DistributionTab::setChargingValue(double value)
{
    ui->lcdCharging->display(QString::number(value, 'f', 2));
}



void DistributionTab::changStateDisconnector(int index, CellWindow::DataCell dataCell)
{
    mRegObject[static_cast<TabIndices>(index)].first->setPixmap(!dataCell.onDisconnector ? QPixmap(":/Resources/Icon/disconnector.png") : QPixmap(":/Resources/Icon/conected.png"));
    mRegObject[static_cast<TabIndices>(index)].first->setAlignment(!dataCell.onDisconnector ? Qt::AlignmentFlag::AlignJustify : Qt::AlignmentFlag::AlignHCenter);
    if(mRegObject[static_cast<TabIndices>(index)].second != nullptr)
    {
        mRegObject[static_cast<TabIndices>(index)].second->setPixmap(!dataCell.onContactor ? QPixmap(":/Resources/Icon/disconnector.png") : QPixmap(":/Resources/Icon/conected.png"));
        mRegObject[static_cast<TabIndices>(index)].second->setAlignment(!dataCell.onContactor ? Qt::AlignmentFlag::AlignJustify : Qt::AlignmentFlag::AlignHCenter);
    }
}

