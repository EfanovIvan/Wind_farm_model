
#include <QString>
#include <QColor>
#include <QStyle>

#include "CellWindow.h"
#include "ui_cell.h"

CellWindow::CellWindow(QDialog  * parent ) : QDialog(parent), ui(new Ui::CellDialog)
{

    qRegisterMetaType<CellWindow::DataCell>( "CellWindow::DataCell" );
    ui->setupUi(this);
    ui->offCircuit->setIndexType(TabIndices::offCircuit);
    ui->onCircuit->setIndexType(TabIndices::onCircuit);
    ui->onContactor->setIndexType(TabIndices::onContactor);
    ui->offContactor->setIndexType(TabIndices::offContactor);


    QObject::connect(ui->offCircuit, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->onCircuit, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->offContactor, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->onContactor, SIGNAL(clicked(int)), this, SLOT(clicked(int)));

    mCellsData.insert(static_cast<int>(TabIndices::WG1), DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::WG2), DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::Diesel), DataCell());
    mCellsData.insert(static_cast<int>(TabIndices::ChargingStation), DataCell());

    QObject::connect(ui->manualButton, SIGNAL(clicked()), this, SLOT(onManualControl()));
    QObject::connect(ui->autoButton, SIGNAL(clicked()), this, SLOT(onAutoControl()));

    ui->currentManagment->setText("Включено автоматическое управление");
    ui->currentManagment->setStyleSheet("color: rgb(0,255,0)");
    ui->autoButton->setEnabled(false);
}

CellWindow::~CellWindow()
{
    delete ui;
}

void CellWindow::setIndexCell(int index)
{
    mCurrentIndex = index;
    auto in = static_cast<TabIndices>(index);
    int cellNumber = 0;
    if(in == TabIndices::WG1 || in == TabIndices::WG2)
    {
        int number = in == TabIndices::WG1 ? 1 : 2;
        ui->nameCell->setText(QString("Ячейка №%1").arg(number));
        ui->nameContactor->setText(QString("КМ%1").arg(number));
        ui->nameDisconnector->setText(QString("P%1").arg(number));
        cellNumber = in == TabIndices::WG1 ? 1 : 2;
    }
    else if(in == TabIndices::Diesel)
    {
        ui->nameCell->setText("Ячейка №7");
        ui->nameContactor->setText("КМ10");
        ui->nameDisconnector->setText("P10");
        cellNumber = 7;
    }
    else if(in == TabIndices::ChargingStation)
    {
        ui->nameCell->setText("Ячейка №6");
        ui->nameContactor->setText("КМ9");
        ui->nameDisconnector->setText("P9");
        cellNumber = 6;
    }
    this->setWindowTitle(QString("Ячейка №%1").arg(cellNumber));
    updateViewCell();
}

void CellWindow::clicked(int index)
{
    auto in = static_cast<TabIndices>(index);
    if(mCellsData[mCurrentIndex].automatic)
    {
        messageBox.reset();
        messageBox = std::make_shared<QMessageBox>();
        messageBox->setText(QString("Переключись на ручное управление!"));
        messageBox->show();
        return;
    }

    if(in == TabIndices::offCircuit || in == TabIndices::onCircuit)
    {
        if(mCellsData[mCurrentIndex].onContactor)
        {
            messageBox.reset();
            messageBox = std::make_shared<QMessageBox>();
            messageBox->setText(TabIndices::offCircuit == in ? QString("Отключи контактор перед отключением рубильника!") : QString("Отключи контактор перед включением рубильника!"));
            messageBox->show();
            return;
        }

         mCellsData[mCurrentIndex].onDisconnector = TabIndices::onCircuit == in;
        emit changStateDisconnector(mCurrentIndex,  mCellsData[mCurrentIndex]);
    }
    else if(in == TabIndices::offContactor || in == TabIndices::onContactor)
    {
        mCellsData[mCurrentIndex].onContactor = TabIndices::onContactor == in;
        emit changStateDisconnector(mCurrentIndex,  mCellsData[mCurrentIndex]);
    }
    updateViewCell();
}

void CellWindow::updateViewCell()
{
    ui->label->setPixmap(!mCellsData[mCurrentIndex].onDisconnector ? QPixmap(":/Resources/Icon/CircuitBreakerOff.png") : QPixmap(":/Resources/Icon/CircuitBreakerOn.png"));
    ui->label_2->setPixmap(!mCellsData[mCurrentIndex].onContactor ? QPixmap(":/Resources/Icon/contactorOff.png") : QPixmap(":/Resources/Icon/contactorOn.png"));
    ui->textConOn->setStyleSheet(mCellsData[mCurrentIndex].onContactor ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textConOff->setStyleSheet(!mCellsData[mCurrentIndex].onContactor ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");
    ui->textDiscOm->setStyleSheet(mCellsData[mCurrentIndex].onDisconnector ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textDiscOff->setStyleSheet(!mCellsData[mCurrentIndex].onDisconnector ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");
}

void CellWindow::updateAllElement()
{
    for(auto it = mCellsData.begin(); it != mCellsData.end(); ++it)
    {
        emit changStateDisconnector(it.key(), *it);
    }
}

QVector<CellWindow::DataCell> CellWindow::getCellData(TabIndices index)
{
    return QVector<CellWindow::DataCell> {mCellsData[static_cast<int>(index)]};
}

void CellWindow::setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData)
{
    int in = static_cast<int>(index);
    mCellsData[in] = cellData.front();
    emit changStateDisconnector(in,  mCellsData[in]);
}

void CellWindow::onAutoControl()
{
    mCellsData[mCurrentIndex].automatic = true;
    ui->autoButton->setEnabled(false);
    ui->manualButton->setEnabled(true);
    ui->currentManagment->setText("Включено автоматическое управление");
    ui->currentManagment->setStyleSheet("color: rgb(0,255,0)");
}

void CellWindow::onManualControl()
{
    mCellsData[mCurrentIndex].automatic = false;
    ui->manualButton->setEnabled(false);
    ui->autoButton->setEnabled(true);
    ui->currentManagment->setText("Включено ручное управление");
    ui->currentManagment->setStyleSheet("color: rgb(255,0,0)");
}
