
#include <QString>
#include <QColor>
#include <QStyle>
#include <array>

#include "CellWindow.h"
#include "DoubleCellWindow.h"
#include "ui_doublecell.h"

DoubleCellWindow::DoubleCellWindow(QDialog  *parent ): QDialog(parent),ui(new Ui::DoubleCell)
{
    ui->setupUi(this);
    ui->offCircuit1->setIndexType(TabIndices::offCircuit);
    ui->onCircuit1->setIndexType(TabIndices::onCircuit);
    ui->onContactor1->setIndexType(TabIndices::onContactor);
    ui->offContactor1->setIndexType(TabIndices::offContactor);

    ui->offCircuit2->setIndexType(TabIndices::offCircuit);
    ui->onCircuit2->setIndexType(TabIndices::onCircuit);
    ui->onContactor2->setIndexType(TabIndices::onContactor);
    ui->offContactor2->setIndexType(TabIndices::offContactor);


    QObject::connect(ui->offCircuit1, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->onCircuit1, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->offContactor1, SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    QObject::connect(ui->onContactor1, SIGNAL(clicked(int)), this, SLOT(clicked(int)));


    QObject::connect(ui->offCircuit2, SIGNAL(clicked(int)), this, SLOT(clickedSecondCell(int)));
    QObject::connect(ui->onCircuit2, SIGNAL(clicked(int)), this, SLOT(clickedSecondCell(int)));
    QObject::connect(ui->offContactor2, SIGNAL(clicked(int)), this, SLOT(clickedSecondCell(int)));
    QObject::connect(ui->onContactor2, SIGNAL(clicked(int)), this, SLOT(clickedSecondCell(int)));

    QObject::connect(ui->manualButton, SIGNAL(clicked()), this, SLOT(onManualControl()));
    QObject::connect(ui->autoButton, SIGNAL(clicked()), this, SLOT(onAutoControl()));

    mCellsData.insert(static_cast<int>(TabIndices::IndustrialSector), {CellWindow::DataCell(),CellWindow::DataCell()});
    mCellsData.insert(static_cast<int>(TabIndices::ResidentialSector), {CellWindow::DataCell(),CellWindow::DataCell()});
    mCellsData.insert(static_cast<int>(TabIndices::LivestockComplex),   {CellWindow::DataCell(),CellWindow::DataCell()});

    ui->currentManagment->setText("Включено автоматическое управление");
    ui->currentManagment->setStyleSheet("color: rgb(0,255,0)");
    ui->autoButton->setEnabled(false);
}

DoubleCellWindow::~DoubleCellWindow()
{
    delete ui;
}

void DoubleCellWindow::setIndexCell(int index)
{
    mCurrentIndex = index;
    auto in = static_cast<TabIndices>(index);
    int cellNumber = 0;
    if(in == TabIndices::IndustrialSector )
    {
        int number = 3;
        ui->nameCell->setText(QString("Ячейка №%1").arg(number));
        ui->nameContactor->setText(QString("КМ%1").arg(number));
        ui->nameDisconnector1->setText(QString("P%1").arg(number));
        ui->nameContactor_2->setText(QString("КМ%1").arg(number + 1));
        ui->nameDisconnector2->setText(QString("P%1").arg(number + 1));
        cellNumber = 3;
    }
    else if(in == TabIndices::ResidentialSector)
    {
        int number = 5;
        cellNumber = 4;
        ui->nameCell->setText(QString("Ячейка №%1").arg(cellNumber));
        ui->nameContactor->setText(QString("КМ%1").arg(number));
        ui->nameDisconnector1->setText(QString("P%1").arg(number));
        ui->nameContactor_2->setText(QString("КМ%1").arg(number + 1));
        ui->nameDisconnector2->setText(QString("P%1").arg(number + 1));

    }
    else if(in == TabIndices::LivestockComplex)
    {
        int number = 7;
        cellNumber = 5;
        ui->nameCell->setText(QString("Ячейка №%1").arg(cellNumber));
        ui->nameContactor->setText(QString("КМ%1").arg(number));
        ui->nameDisconnector1->setText(QString("P%1").arg(number));
        ui->nameContactor_2->setText(QString("КМ%1").arg(number + 1));
        ui->nameDisconnector2->setText(QString("P%1").arg(number + 1));

    }
    this->setWindowTitle(QString("Ячейка №%1").arg(cellNumber));
    updateViewCell();
}

void DoubleCellWindow::updateAllElement()
{
    for(auto it = mCellsData.begin(); it != mCellsData.end(); ++it)
    {
        emit changStateDisconnector(static_cast<int>(it.key()),  (*it)[0]);
        emit changStateDisconnector(static_cast<int>(it.key()) + 17,  (*it)[1]);
    }
}

QVector<CellWindow::DataCell> DoubleCellWindow::getCellData(TabIndices index)
{
    return QVector<CellWindow::DataCell> { mCellsData[static_cast<int>(index)][0], mCellsData[static_cast<int>(index)][1]};
}

void DoubleCellWindow::setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData)
{
    int in = static_cast<int>(index);
    mCellsData[in][0] = cellData[0];
    mCellsData[in][1] = cellData[1];
    emit changStateDisconnector(in,  mCellsData[in][0]);
    emit changStateDisconnector(in + 17,  mCellsData[in][1]);
}

void DoubleCellWindow::clicked(int index)
{
    auto in = static_cast<TabIndices>(index);

    if(mCellsData[mCurrentIndex][1].automatic)
    {
        messageBox.reset();
        messageBox = std::make_shared<QMessageBox>();
        messageBox->setText(QString("Переключись на ручное управление!"));
        messageBox->show();
        return;
    }

    if(in == TabIndices::offCircuit || in == TabIndices::onCircuit)
    {
        if(mCellsData[mCurrentIndex][0].onContactor)
        {
            messageBox.reset();
            messageBox = std::make_shared<QMessageBox>();
            messageBox->setText(TabIndices::offCircuit == in ? QString("Отключи контактор перед отключением рубильника!") : QString("Отключи контактор перед включением рубильника!"));
            messageBox->show();
            return;
        }

         mCellsData[mCurrentIndex][0].onDisconnector = TabIndices::onCircuit == in;
        emit changStateDisconnector(mCurrentIndex,  mCellsData[mCurrentIndex][0]);
    }
    else if(in == TabIndices::offContactor || in == TabIndices::onContactor)
    {
        mCellsData[mCurrentIndex][0].onContactor = TabIndices::onContactor == in;
        emit changStateDisconnector(mCurrentIndex,  mCellsData[mCurrentIndex][0]);
    }
    updateViewCell();
}

void DoubleCellWindow::clickedSecondCell(int index)
{
     auto in = static_cast<TabIndices>(index);

     if(mCellsData[mCurrentIndex][1].automatic)
     {
         messageBox.reset();
         messageBox = std::make_shared<QMessageBox>();
         messageBox->setText(QString("Переключись на ручное управление!"));
         messageBox->show();
         return;
     }

     if(in == TabIndices::offCircuit || in == TabIndices::onCircuit)
     {
         if(mCellsData[mCurrentIndex][1].onContactor)
         {
             messageBox.reset();
             messageBox = std::make_shared<QMessageBox>();
             messageBox->setText(TabIndices::offCircuit == in ? QString("Отключи контактор перед отключением рубильника!") : QString("Отключи контактор перед включением рубильника!"));
             messageBox->show();
             return;
         }

          mCellsData[mCurrentIndex][1].onDisconnector = TabIndices::onCircuit == in;
         emit changStateDisconnector( mCurrentIndex + 17,  mCellsData[mCurrentIndex][1]);
     }
     else if(in == TabIndices::offContactor || in == TabIndices::onContactor)
     {
         mCellsData[mCurrentIndex][1].onContactor = TabIndices::onContactor == in;
         emit changStateDisconnector(mCurrentIndex + 17,  mCellsData[mCurrentIndex][1]);
     }

    updateViewCell();
}

void DoubleCellWindow::updateViewCell()
{
    ui->label1->setPixmap(!mCellsData[mCurrentIndex][0].onDisconnector ? QPixmap(":/Resources/Icon/CircuitBreakerOff.png") : QPixmap(":/Resources/Icon/CircuitBreakerOn.png"));
    ui->label2->setPixmap(!mCellsData[mCurrentIndex][0].onContactor ? QPixmap(":/Resources/Icon/contactorOff.png") : QPixmap(":/Resources/Icon/contactorOn.png"));
    ui->textConOn1->setStyleSheet(mCellsData[mCurrentIndex][0].onContactor ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textConOff1->setStyleSheet(!mCellsData[mCurrentIndex][0].onContactor ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");
    ui->textDiscOn1->setStyleSheet(mCellsData[mCurrentIndex][0].onDisconnector ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textDiscOff1->setStyleSheet(!mCellsData[mCurrentIndex][0].onDisconnector ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");

    ui->label2_1->setPixmap(!mCellsData[mCurrentIndex][1].onDisconnector ? QPixmap(":/Resources/Icon/CircuitBreakerOff.png") : QPixmap(":/Resources/Icon/CircuitBreakerOn.png"));
    ui->label2_2->setPixmap(!mCellsData[mCurrentIndex][1].onContactor ? QPixmap(":/Resources/Icon/contactorOff.png") : QPixmap(":/Resources/Icon/contactorOn.png"));
    ui->textConOn2->setStyleSheet(mCellsData[mCurrentIndex][1].onContactor ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textConOff2->setStyleSheet(!mCellsData[mCurrentIndex][1].onContactor ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");
    ui->textDiscOn2->setStyleSheet(mCellsData[mCurrentIndex][1].onDisconnector ? "color: rgb(255,0,0)" : "color: rgb(192,192,192)");
    ui->textDiscOff2->setStyleSheet(!mCellsData[mCurrentIndex][1].onDisconnector ? "color: rgb(0,255,0)" : "color: rgb(192,192,192)");

}

void DoubleCellWindow::onAutoControl()
{
    mCellsData[mCurrentIndex][0].automatic = true;
    mCellsData[mCurrentIndex][1].automatic = true;
    ui->autoButton->setEnabled(false);
    ui->manualButton->setEnabled(true);
    ui->currentManagment->setText("Включено автоматическое управление");
    ui->currentManagment->setStyleSheet("color: rgb(0,255,0)");
}

void DoubleCellWindow::onManualControl()
{
    mCellsData[mCurrentIndex][0].automatic = false;
    mCellsData[mCurrentIndex][1].automatic = false;
    ui->manualButton->setEnabled(false);
    ui->autoButton->setEnabled(true);
    ui->currentManagment->setText("Включено ручное управление");
    ui->currentManagment->setStyleSheet("color: rgb(255,0,0)");
}
