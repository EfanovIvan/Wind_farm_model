#ifndef INDUSTRIALTAB_H
#define INDUSTRIALTAB_H
#include <QWidget>
#include <array>

#include "tabs/LineTab.h"
#include "utils/LoadDataFromFile.h"
#include "models/ProxyIndustrialModel.h"

class TableCoefficientsPower;
namespace Ui {
    class Industrial;
}



class IndustrialTab : public QWidget, public LoadDataFromFile
{
    Q_OBJECT
public:
    explicit IndustrialTab(QWidget *parent = nullptr);
    ~IndustrialTab();
    void setModel(TableCoefficientsPower * tableCoefficients);
    double getPower(int currentMonth, int currentHour);
    double getPower();
    void loadDataFromFile(const QString path) override;
    void saveInFile(QJsonObject & root) override;
signals:
    void changePower(double, LineTab::NumberLine);
    void updatePower(TabIndices, double);
public slots:
       void drawGraph(QVector<double> k, int type);
       void recountPower(double power, int type);
private:
    Ui::Industrial * ui;
    TableCoefficientsPower * tableCoeff;
    std::array<QVector<double>, 2>     mCoefficients;
};

#endif // INDUSTRIALTAB_H
