#ifndef WINDGENERATORTAB_H
#define WINDGENERATORTAB_H

#include <QWidget>
#include <array>
#include <memory>

#include "models/WGProxyModel.h"
#include "utils/CheckSelection.h"
#include "utils/LoadDataFromFile.h"
#include "tabs/LineTab.h"
#include "windows/SpeedPowerWindow.h"

namespace Ui
{
    class WindGenerator;
}
class WindGeneratorTab : public QWidget, public CheckSelection, public LoadDataFromFile
{
    Q_OBJECT
public:
    explicit WindGeneratorTab(QWidget *parent = nullptr);
    ~WindGeneratorTab();
    WGProxyModel::DataWG getDataWG1();
    WGProxyModel::DataWG getDataWG2();
    std::shared_ptr<QMessageBox> check() override;
    void loadDataFromFile(const QString path) override;
    void saveInFile(QJsonObject & root) override;
    void sumPowerWG();
signals:
    void changeModelValidIcon(TabIndices, bool );
    void changePower(double);
    void changePower(double power, LineTab::NumberLine line);
public slots:
    void changeMarkWG1(int);
    void changeMarkWG2(int);
    void setSpeedPowerWG1();
    void setSpeedPowerWG2();
    void OnSpeedPowerWindow(SpeedPowerModel::Dependence wg);
private:
    Ui::WindGenerator * ui;
    SpeedPowerWindow   mSpeedPowerWindow;
    std::array<bool, 2> mValidWG;
};

#endif // WINDGENERATORTAB_H
