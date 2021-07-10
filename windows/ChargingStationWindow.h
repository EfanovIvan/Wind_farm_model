#ifndef CHARGINGSTATIONWINDOW_H
#define CHARGINGSTATIONWINDOW_H

#include <QDialog>
namespace Ui {
    class ChargingStation;
}

class ChargingStationWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ChargingStationWindow(QDialog *parent = nullptr);
    ~ChargingStationWindow();
    void setChargingValue(double value);

private:
    Ui::ChargingStation * ui;
};

#endif // CHARGINGSTATIONWINDOW_H
