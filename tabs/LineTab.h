#ifndef LINETAB_H
#define LINETAB_H
#include <array>
#include <QWidget>
#include <QMap>
#include <QIdentityProxyModel>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "utils/CheckSelection.h"
#include "tabs/TabIndex.h"
#define SQRT3 1.73205
namespace Ui
{
    class Form;
}

class LineTab : public QWidget, public CheckSelection
{
    Q_OBJECT
public:

    enum NumberLine
    {
        LINE_1_RESIDENTIAL = 0,
        LINE_2_LIVESTOCK,
        LINE_3_INDUSTRIAL,
        LINE_WG1,
        LINE_WG2,
        NONE
    };

    struct DataWire
    {
        DataWire():name(""),Ilong(0),r(0.0),x(0.0),x0(0.0),r0(0.0),withNullWire(false),onlyWG(false){}
        QString name;
        int     Ilong;
        double   r;
        double   x;
        double   r0;
        double   x0;
        bool     withNullWire;
        bool     onlyWG;
    };

    struct DataLine
    {
        DataLine():powerLine(0.0),cosFi(1.0), length(0.0),fine(false),ZloopPhaseZero(0.0),ZloopWGLine(0.0){}
        double powerLine;
        double cosFi;
        double length;
        DataWire dataWire;
        bool fine;
        double ZloopPhaseZero;
        double ZloopWGLine;
        QComboBox * comBox2;
        QLabel * Icount;
        QLabel * delU;
        QLabel * cond1;
        QLabel * cond2;
        QLabel * cond3;
        QLineEdit * uiCos;
        QLineEdit * uiLength;
        QLabel * uiText;
        QLabel * uiValidIcon;
    };

    explicit LineTab(QWidget *parent = nullptr);
    std::shared_ptr<QMessageBox> check() override;
    void updateDelU();
    void updateValidIcon(const QString & name);

signals:
    void changeModelValidIcon(TabIndices, bool);
    void sendDataLine(TabIndices, LineTab::DataLine);

public slots:
    void updatePower(double, LineTab::NumberLine);
    void recountLine();
    void recountLine(QString name);
    void setIndexLine(QString name);
private:
    void loadDataFromJson();
    QStringList getNamesWires();
    QStringList getNamesNullWires();
    QStringList getNamesWiresWG();
    void getCosAndLength();
    void updateWire(const QString & name);
    void setTextLable();
        //QStringList getNamesWires(bool nullWire = false, bool onlyWG = false);

    void setObjectLine(NumberLine line, QComboBox * comBox2, QLabel * Icount, QLabel * delU, QLabel * cond1, QLabel * cond2, QLabel * cond3, QLineEdit * cos, QLineEdit * length, QLabel * text, QLabel * validIcon);
private:
    Ui::Form *                 ui;
    std::array<DataLine, 5>    mDataLines;
    NumberLine                 mCurrentLine;
    QMap<QString, DataWire>    mDataWires;
    QMap<QObject*, NumberLine> mRegistedObject;
    double                     mDelUWG;
};
Q_DECLARE_METATYPE(LineTab::DataLine);

#endif // LINETAB_H
