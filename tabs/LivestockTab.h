#ifndef LIVESTOCK_H
#define LIVESTOCK_H
#include <QWidget>
#include <array>

#include "utils/LoadDataFromFile.h"
#include "tabs/LineTab.h"

class TableCoefficientsPower;
namespace Ui
{
    class Livestock;
}

class Livestock : public QWidget, public LoadDataFromFile
{
    Q_OBJECT
public:
    explicit Livestock(QWidget *parent = nullptr);
    ~Livestock() override;
    void setModel(TableCoefficientsPower * tableCoefficients);
    double getPower(int currentMonth, int currentHour);
    double getPower();
     void loadDataFromFile(const QString path) override;
     void saveInFile(QJsonObject & root) override;
signals:
    void changePower(double, LineTab::NumberLine);
    void updatePower(TabIndices, double);
public slots:
    void drawGraph(QVector<double> k, int sumPower);
    void recountPower();
private:
    Ui::Livestock * ui;
    TableCoefficientsPower * tableCoeff;
    std::array<QVector<double>, 2>     mCoefficients;
};

#endif // LIVESTOCK_H
