
#include <array>

#include "windows/SpeedPowerWindow.h"
#include "ui_speedpower.h"
#include "models/SpeedPowerModel.h"
#include "utils/CoefficientValidator.h"


SpeedPowerWindow::SpeedPowerWindow(QDialog * parent) : QDialog(parent), ui(new Ui::SpeedPower)
{

    ui->setupUi(this);
    mSpeedPower[SpeedPowerModel::WG1] = new SpeedPowerModel(this);
    mSpeedPower[SpeedPowerModel::WG2] = new SpeedPowerModel(this);
}

SpeedPowerWindow::~SpeedPowerWindow()
{
    delete ui;
}

void SpeedPowerWindow::setDependence(SpeedPowerModel::Dependence dependence)
{
   mDependence = dependence;
   auto wg = mSpeedPower[mDependence];
   ui->tableView->setModel(wg);
   ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
   auto validator = new CoefficientValidator(this, 0.00, 1000.0);
    ui->tableView->setItemDelegate(validator);
}

void SpeedPowerWindow::setTitleText(QString text)
{
    this->setWindowTitle("График мощности ВА " + text);
}

QMap<int, double> SpeedPowerWindow::getSpeedPower(SpeedPowerModel::Dependence dependence)
{
    return mSpeedPower[dependence]->getSpeedDependence();
}
