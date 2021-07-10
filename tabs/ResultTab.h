#ifndef RESULTTAB_H
#define RESULTTAB_H

#include <QWidget>
#include <memory>

#include "utils/LoadDataFromFile.h"
#include "utils/CheckSelection.h"

namespace Ui
{
    class Result;
}

class ResultTab : public QWidget, public LoadDataFromFile, public CheckSelection
{
    Q_OBJECT
public:
    explicit ResultTab(QWidget *parent = nullptr);
    ~ResultTab() override;
     std::shared_ptr<QMessageBox> check() override;

    void setCalculationData(const std::array<QVector<QVariant>,3> & calculateData);
    void clearData();
    QPair<double, double> getBeaufortRange(int numberOfDay);
    void loadDataFromFile(const QString path) override;
    void saveInFile(QJsonObject & root) override;
    void initGraph();
signals:

public slots:
    void expotExel();
    void setSumWGEnergy(double);
    void setSumDiselEnergy(double );
    void setTotalSumEnergy(double );
private:
    Ui::Result * ui;
};

#endif // RESULTTAB_H
