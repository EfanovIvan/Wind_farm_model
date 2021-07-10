
#include "CellWindowWithoutContactor.h"
#include "ui_disconnectorcell.h"
#include "CellWindow.h"

CellWindowWithoutContactor::CellWindowWithoutContactor(QDialog *parent) : QDialog(parent), ui(new Ui::Disconnector)
{
    ui->setupUi(this);
    ui->offCircuit->setIndexType(TabIndices::offCircuit);
    ui->onCircuit->setIndexType(TabIndices::onCircuit);

    QObject::connect(ui->offCircuit, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->onCircuit, SIGNAL(clicked(int)), this, SLOT(clicked(int)));

    mCellsData.insert(static_cast<int>(TabIndices::Shop), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::Garage), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::Greenhouse), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::BirdHouse), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::Farm), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::Building), CellWindow::DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::AdminBuilding), CellWindow::DataCell());
}

CellWindowWithoutContactor::~CellWindowWithoutContactor()
{
    delete ui;
}

void CellWindowWithoutContactor::clicked(int index)
{
    auto in = static_cast<TabIndices>(index);
    if(in == TabIndices::offCircuit || in == TabIndices::onCircuit)
    {
         mCellsData[mCurrentIndex].onDisconnector = TabIndices::onCircuit == in;
        emit changStateDisconnector(mCurrentIndex,  mCellsData[mCurrentIndex]);
    }
    updateViewCell();
}

void CellWindowWithoutContactor::updateViewCell()
{
    ui->label->setPixmap(!mCellsData[mCurrentIndex].onDisconnector ? QPixmap(":/Resources/Icon/CircuitBreakerOff.png") : QPixmap(":/Resources/Icon/CircuitBreakerOn.png"));
    ui->textDiscOm->setStyleSheet(mCellsData[mCurrentIndex].onDisconnector ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textDiscOff->setStyleSheet(!mCellsData[mCurrentIndex].onDisconnector ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");
}

void CellWindowWithoutContactor::setIndexCell(int index)
{
    mCurrentIndex = index;
    auto in = static_cast<TabIndices>(index);
    int cellNumber = 0;
    if(in == TabIndices::Shop || in == TabIndices::Garage ||in == TabIndices::Greenhouse)
    {
        ui->nameCell->setText(QString("Ячейка №3"));
        if(in == TabIndices::Shop)
        {
            ui->nameDisconnector->setText(QString("P11"));
        }
        else if(in == TabIndices::Garage)
        {
            ui->nameDisconnector->setText(QString("P12"));
        }
        else if(in == TabIndices::Greenhouse)
        {
            ui->nameDisconnector->setText(QString("P13"));
        }
        cellNumber = 3;
    }
    else if(in == TabIndices::Building)
    {
        ui->nameCell->setText(QString("Ячейка №4"));
        ui->nameDisconnector->setText(QString("P14"));
    }
    else if(in == TabIndices::Farm || in == TabIndices::BirdHouse )
    {
        ui->nameCell->setText("Ячейка №5");
        if(in == TabIndices::Farm)
        {
             ui->nameDisconnector->setText("P15");
        }
        else if(in == TabIndices::BirdHouse)
        {
          ui->nameDisconnector->setText("P16");
        }
    }
    updateViewCell();
}

QVector<CellWindow::DataCell> CellWindowWithoutContactor::getCellData(TabIndices index)
{
    return QVector<CellWindow::DataCell> {mCellsData[static_cast<int>(index)]};
}

void CellWindowWithoutContactor::setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData)
{
    int in = static_cast<int>(index);
    mCellsData[in] = cellData[0];
    emit changStateDisconnector(in,  mCellsData[in]);
}
