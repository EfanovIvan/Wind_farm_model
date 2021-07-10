#ifndef FUSEWINDOW_H
#define FUSEWINDOW_H
#include <QDialog>
#include <array>
#include <QMap>
#include <memory>

#include "tabs/TabIndex.h"
#include "tabs/LineTab.h"

namespace Ui {
class Dialog;
}
class FuseModel;
class DistributionTab;
class FuseWindow : public QDialog
{
    Q_OBJECT
public:
    struct ParamFuse
    {
      ParamFuse():Inomfuse(0),Imelt(0),indexKeyFuse(0),indexKeyMelt(0),conditionsMet(false),accepted(false){}
      int Inomfuse;
      int Imelt;
      int indexKeyFuse;
      int indexKeyMelt;
      bool conditionsMet;
      bool accepted;
    };

    struct ParamLine
    {
        ParamLine():I_longValidLine(0.0),ZloopPZ(0.0){}
        double I_longValidLine;
        double ZloopPZ;
    };

    FuseWindow(QDialog  *parent = nullptr);
    ~FuseWindow();
    void setLineNumber(int index);
     void setParamLine(TabIndices line, LineTab::DataLine);
     void setSumPowerWG(double sumPowerWG) { mSumPowerWG = sumPowerWG;}
     void setParent(DistributionTab * par) { parent = par; }
     bool checkFuse();
public slots:
    void changeMelt(int index);
    void setSelectedData(int index);
    void setPower(TabIndices line, double power) { mPowerLines[line] = power;}


private:
    void recountParam();
private:
    Ui::Dialog *                           mUiFuse;
    TabIndices                             mIndexLine;
    FuseModel *                            model;
    QMap<TabIndices, ParamFuse>            mParamFuse;
    TabIndices                             mIndexData;
    QMap<TabIndices, double>               mPowerLines;
    QMap<TabIndices, LineTab::DataLine>    mDataLine;
    double                                 mSumPowerWG;
    DistributionTab  *                     parent;
};

#endif // FUSEWINDOW_H
