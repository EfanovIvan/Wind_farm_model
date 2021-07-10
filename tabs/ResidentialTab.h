#ifndef RESIDENTIALTAB_H
#define RESIDENTIALTAB_H

#include <QWidget>
#include <array>

#include "utils/LoadDataFromFile.h"
#include "tabs/LineTab.h"

class TableCoefficientsPower;
namespace Ui
{
    class Residential;
}

class ResidentialTab : public QWidget, public LoadDataFromFile
{
    Q_OBJECT
public:
    explicit ResidentialTab(QWidget *parent = nullptr);
    ~ResidentialTab();
    void setModel(TableCoefficientsPower * tableCoefficients);
    double getPower(int currentMonth, int currentHour);
    void loadDataFromFile(const QString path) override;
    void saveInFile(QJsonObject & root) override;
    double getPower();

signals:
    void changePower(double, LineTab::NumberLine);
    void  updatePower(TabIndices, double);
public slots:
       void drawGraph(QVector<double> k, int type);
       void recountPower();
private:
    Ui::Residential * ui;
    std::array<QVector<double>, 2>     mCoefficients;
};

#endif // RESIDENTIALTAB_H
