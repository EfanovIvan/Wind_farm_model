#ifndef SPEEDPOWERWINDOW_H
#define SPEEDPOWERWINDOW_H

#include <QDialog>
#include <array>

#include "models/SpeedPowerModel.h"

namespace Ui {
class SpeedPower;
}

class SpeedPowerWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SpeedPowerWindow( QDialog *parent = nullptr);
    ~SpeedPowerWindow();
    void setDependence(SpeedPowerModel::Dependence dependence);
    void setTitleText(QString text);
    QMap<int, double> getSpeedPower(SpeedPowerModel::Dependence dependence);
signals:

public slots:

private:
    Ui::SpeedPower * ui;
    std::array<SpeedPowerModel*, 2> mSpeedPower;
    SpeedPowerModel::Dependence    mDependence;
};

#endif // SPEEDPOWERWINDOW_H
