#include "ChargingStationWindow.h"
#include "ui_chargingstation.h"

ChargingStationWindow::ChargingStationWindow(QDialog *parent) : QDialog(parent), ui(new Ui::ChargingStation)
{
    ui->setupUi(this);
    this->setWindowTitle("Зарядная станция");
}

ChargingStationWindow::~ChargingStationWindow()
{
    delete ui;
}

void ChargingStationWindow::setChargingValue(double value)
{
    ui->lcdNumber->display(QString::number(value, 'f', 2));
}
