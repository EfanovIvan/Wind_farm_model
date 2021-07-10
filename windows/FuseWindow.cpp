#include <array>
#include <math.h>
#include <stdio.h>

#include "windows/FuseWindow.h"
#include "ui_fuse.h"
#include "models/FuseModel.h"
#include "tabs/DistributionTab.h"
#include "ui_distribution.h"

FuseWindow::FuseWindow(QDialog  *parent ):QDialog(parent), mUiFuse(new Ui::Dialog)
{
    mUiFuse->setupUi(this);
    model = new FuseModel(this);
    mUiFuse->FusesBox->addItems(model->getFuseList());
    QObject::connect(mUiFuse->FusesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMelt(int)));
    QObject::connect(mUiFuse->MeltBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedData(int)));

    mUiFuse->InputIFUse->setVisible(false);
    mUiFuse->InputIMelt->setVisible(false);
    mUiFuse->FuseEdite->setVisible(false);
    mUiFuse->MeltEdit->setVisible(false);
}

FuseWindow::~FuseWindow()
{
    delete mUiFuse;
}

void FuseWindow::setLineNumber(int index)
{
    mIndexLine = static_cast<TabIndices>(index);
    if(mIndexLine == TabIndices::Line3)
    {
        mUiFuse->Text->setText(QString("Предохранитель ПП4 и ПП5 (Линия 3)"));
        mIndexData = TabIndices::Line3;
    }
    else if(mIndexLine == TabIndices::Line2)
    {
        mUiFuse->Text->setText(QString("Предохранитель ПП6 и ПП7 (Линия 2)"));
        mIndexData = TabIndices::Line2;
    }
    else if(mIndexLine == TabIndices::Line1)
    {
        mUiFuse->Text->setText(QString("Предохранитель ПП8 и ПП9 (Линия 1)"));
        mIndexData = TabIndices::Line1;
    }
    this->setWindowTitle("Предохранитель");

    mUiFuse->FusesBox->setCurrentIndex(mParamFuse[mIndexData].indexKeyFuse);
    mUiFuse->MeltBox->setCurrentIndex(mParamFuse[mIndexData].indexKeyMelt);
    recountParam();
}

void FuseWindow::changeMelt(int index)
{

    mUiFuse->MeltBox->clear();
    mUiFuse->InputIFUse->setVisible(index == 0);
    mUiFuse->InputIMelt->setVisible(index == 0);
    mUiFuse->FuseEdite->setVisible(index == 0);
    mUiFuse->MeltEdit->setVisible(index == 0);

    mUiFuse->ImageFuse->setPixmap(model->getImageFuse(index));
    mUiFuse->ImageMelt->setPixmap(model->getImageMelt(index));

    mParamFuse[mIndexData].indexKeyFuse = index;
    mParamFuse[mIndexData].Inomfuse = model->getCurrentFuse(index);

    if(index > 0)
    {
        mUiFuse->MeltBox->addItems(model->getCurrentMelts(index));
        mParamFuse[mIndexData].Imelt =model->getCurrentMelt(index, 0);
        mParamFuse[mIndexData].indexKeyMelt = 0;
        recountParam();
    }

}

void FuseWindow::setSelectedData(int index)
{
    if(mParamFuse[mIndexData].indexKeyFuse > 0 && index != -1)
    {
        mParamFuse[mIndexData].Imelt = model->getCurrentMelt(mParamFuse[mIndexData].indexKeyFuse, index);
        mParamFuse[mIndexData].indexKeyMelt = index;
        recountParam();
    }
}

void FuseWindow::setParamLine(TabIndices line, LineTab::DataLine dataLine)
{
    mDataLine[line] = dataLine;

}

bool FuseWindow::checkFuse()
{
    for(auto it = mParamFuse.begin(); it != mParamFuse.end(); ++it)
    {
        if(!it->accepted)
        {
            return true;
        }
    }
    return false;
}

void FuseWindow::recountParam()
{
    double I = mPowerLines[mIndexData] * 1000 / (380.0 * SQRT3 * mDataLine[mIndexData].cosFi);

    double Zr0 = (0.03 * pow(400,2) * 0.9) / mSumPowerWG; //0.03 Xo = 0.15 * Xd = 0.15*0.2 = 0.03 o.e.



    double Isc = 220 / (  (Zr0/3 + mDataLine[mIndexData].ZloopWGLine + mDataLine[mIndexData].ZloopPhaseZero));
    if(Isc < 0)
    {
        Isc = 0.0;
    }

    QString Icalculated;
    Icalculated.setNum(I , 'f', 2);

    QChar minOrEqual(0x2A7D);
    mUiFuse->conditionInom->setText(QString("Iрасч = %1 А < Iном.пред = %2 А").arg(Icalculated).arg(mParamFuse[mIndexData].Inomfuse));
    mUiFuse->cond1->setPixmap(I > mParamFuse[mIndexData].Inomfuse ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));

 //   double Icalc = 5*I/2.5;
 //   arg.setNum(Icalc, 'f', 2);
//    mUiFuse->condIcalc->setText(QString("Iпл.вст. = %1 А > 5Iрасч/2.5 = %2 А").arg(mParamFuse[mIndexData].Imelt).arg(arg));
//    mUiFuse->cond2->setPixmap(Icalc > mParamFuse[mIndexData].Imelt ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));

    mUiFuse->condIcalc->setText(QString("Iпл.вст. = %1 А > 1.1Iрасч = %2 А").arg(mParamFuse[mIndexData].Imelt).arg(QString::number(I*1.1 , 'f',2)));
    mUiFuse->cond2->setPixmap(mParamFuse[mIndexData].Imelt > I * 1.1 ? QPixmap(":/Resources/Icon/accept2.png") : QPixmap(":/Resources/Icon/warning4.png"));

    mUiFuse->condIlong->setText(QString("Iпл.вст. = %1 А %2 3Iдл.доп.лин = %3 А").arg(mParamFuse[mIndexData].Imelt).arg(minOrEqual).arg(mDataLine[mIndexData].dataWire.Ilong * 3));
    mUiFuse->cond3->setPixmap(mParamFuse[mIndexData].Imelt <= mDataLine[mIndexData].dataWire.Ilong * 3 ? QPixmap(":/Resources/Icon/accept2.png") : QPixmap(":/Resources/Icon/warning4.png"));

    mUiFuse->condIsc->setText(QString("Iпл.вст. = %1 А < Iкз линии/3 = %2 А").arg(mParamFuse[mIndexData].Imelt).arg(Isc));
    mUiFuse->cond4->setPixmap( mParamFuse[mIndexData].Imelt < Isc ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
    //mParamFuse[mIndexData].accepted = Icalc < mParamFuse[mIndexData].Imelt && I < mParamFuse[mIndexData].Inomfuse && mParamFuse[mIndexData].Imelt > mLongValidLineI[mIndexData];

    bool setAcceptPixmap = false;
    mParamFuse[mIndexData].accepted = I < mParamFuse[mIndexData].Inomfuse
            && mParamFuse[mIndexData].Imelt > I * 1.1
            && mParamFuse[mIndexData].Imelt <= mDataLine[mIndexData].dataWire.Ilong * 3
            &&  mParamFuse[mIndexData].Imelt < Isc;

    if(parent)
    {
        if(mParamFuse[mIndexData].accepted)
        {
            setAcceptPixmap = true;
        }

        if(mIndexData == TabIndices::Line1)
        {
            parent->getUi()->line1_1->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
            parent->getUi()->line1_2->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
        }
        else if(mIndexData == TabIndices::Line3)
        {
            parent->getUi()->line3_1->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
            parent->getUi()->line3_2->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
        }
        else if(mIndexData == TabIndices::Line2)
        {
            parent->getUi()->line2_1->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
            parent->getUi()->line2_2->setPixmap(setAcceptPixmap ?  QPixmap(":/Resources/Icon/accept2.png"): QPixmap(":/Resources/Icon/warning4.png"));
        }
    }
}


