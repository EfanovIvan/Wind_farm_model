#ifndef DISTRIBUTIONTAB_H
#define DISTRIBUTIONTAB_H
#include <QMap>
#include <QPair>
#include <QWidget>

#include "windows/FuseWindow.h"
#include "windows/CellWindow.h"
#include "ClickableLabel.h"
#include "windows/DoubleCellWindow.h"
#include "windows/CellWindowWithoutContactor.h"
#include "utils/CheckSelection.h"

namespace Ui
{
    class Distribution;
}

class DistributionTab : public QWidget, public CheckSelection
{
    Q_OBJECT
public:
    explicit DistributionTab(QWidget *parent = nullptr);
    ~DistributionTab() override;
    Ui::Distribution * getUi() { return ui;}
    std::shared_ptr<QMessageBox> check() override;
signals:

public slots:
       void opentFuseWindow(int index);
       void opentCellWindow(int index);
       void changStateDisconnector(int, CellWindow::DataCell);
       void opentDoubleCellWindow(int index);
       void opentCellWindowWithoutContactor(int index);
       QVector<CellWindow::DataCell> getCellData(TabIndices index);
       void setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData);
       void setPowerLine(TabIndices line, double power);
       void setDataLine(TabIndices, LineTab::DataLine dataLine);
       void changePowerWG(double);
       void setChargingValue(double );
private:
    Ui::Distribution * ui;
    FuseWindow mFuseWindow;
    CellWindow mCellWindow;
    DoubleCellWindow mDoubleCellWindow;
    CellWindowWithoutContactor mCellWindowWithoutContactor;
    QMap<TabIndices, QPair<ClickableLabel *, ClickableLabel *>> mRegObject;
    double mSumPowerWG;
};

#endif // DISTRIBUTIONTAB_H
