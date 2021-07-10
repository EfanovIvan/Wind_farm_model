
#include <memory>
#include <QMessageBox>
#include <QFile>
#include <QTextCodec>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "tabs/WindGeneratorTab.h"
#include "ui_windgenerator.h"
#include "models/WGModel.h"
#include "models/WGProxyModel.h"
#include "models/ProxySelectGeneratorModel.h"
#include "utils/CoefficientValidator.h"
#include "windows/SpeedPowerWindow.h"

WindGeneratorTab::WindGeneratorTab(QWidget *parent) : QWidget(parent), ui(new Ui::WindGenerator)
{
        ui->setupUi(this);

        mValidWG = {false, false};
    WGModel * modelWG = new WGModel(this);
    ui->tableView_WG_1->setModel(new WGProxyModel(modelWG, this));
    ui->tableView_WG_2->setModel(new WGProxyModel(modelWG, this));

    auto validator = new CoefficientValidator(this, 0.00, 1000.0);
    ui->tableView_WG_1->setItemDelegate(validator);
    ui->tableView_WG_2->setItemDelegate(validator);

    ui->tableView_WG_1->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableView_WG_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");

    ui->tableView_WG_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_WG_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_WG_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_WG_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->mSelectWG_1->setModel(new ProxySelectGeneratorModel(modelWG, 16, 14, this));
    ui->mSelectWG_2->setModel(new ProxySelectGeneratorModel(modelWG, 16, 14, this));

    QObject::connect(ui->mSelectWG_1, SIGNAL(activated(int)), ui->tableView_WG_1->model(), SLOT(chageIndexColumn(int)));
    QObject::connect(ui->mSelectWG_2, SIGNAL(activated(int)), ui->tableView_WG_2->model(), SLOT(chageIndexColumn(int)));

    QObject::connect(ui->mSelectWG_1, SIGNAL(activated(int)), ui->mSelectWG_1->model(), SLOT(changeIndexImage(int)));
    QObject::connect(ui->mSelectWG_2, SIGNAL(activated(int)), ui->mSelectWG_2->model(), SLOT(changeIndexImage(int)));

    QObject::connect(ui->mSelectWG_1->model(), SIGNAL(changeImage(QPixmap )), ui->mImageWG_1, SLOT(setPixmap(QPixmap)));
    QObject::connect(ui->mSelectWG_2->model(), SIGNAL(changeImage(QPixmap )), ui->mImageWG_2, SLOT(setPixmap(QPixmap)));


    QObject::connect(ui->mSelectWG_1, SIGNAL(activated(int)), this, SLOT(changeMarkWG1(int)));
    QObject::connect(ui->mSelectWG_2, SIGNAL(activated(int)), this, SLOT(changeMarkWG2(int)));


    QObject::connect(ui->speedPowerButton1, SIGNAL(clicked()), this, SLOT(setSpeedPowerWG1()));
    QObject::connect(ui->speedPowerButton2, SIGNAL(clicked()), this, SLOT(setSpeedPowerWG2()));

    ui->speedPowerButton1->setEnabled(false);
    ui->speedPowerButton2->setEnabled(false);
}

WindGeneratorTab::~WindGeneratorTab()
{
    delete ui;
}

WGProxyModel::DataWG WindGeneratorTab::getDataWG1()
{
    auto proxy = qobject_cast<WGProxyModel*>(ui->tableView_WG_1->model());
    auto dataWG = proxy->getDataWG();
    if(ui->mSelectWG_1->currentIndex() == 1)
    {
         dataWG.speedPowerDependence = mSpeedPowerWindow.getSpeedPower(SpeedPowerModel::Dependence::WG1);
    }
    return dataWG;
}

WGProxyModel::DataWG WindGeneratorTab::getDataWG2()
{
    auto proxy = qobject_cast<WGProxyModel*>(ui->tableView_WG_2->model());
    auto dataWG = proxy->getDataWG();
    if(ui->mSelectWG_2->currentIndex() == 1)
    {
        dataWG.speedPowerDependence = mSpeedPowerWindow.getSpeedPower(SpeedPowerModel::Dependence::WG2);
    }
    return dataWG;
}

std::shared_ptr<QMessageBox> WindGeneratorTab::check()
{
    int i = 1;
    for(auto valid : mValidWG)
    {
        if(!valid)
        {
            auto message = std::make_shared<QMessageBox>();
            message->setText(QString("Нужно выбрать ветрогенератор № %1").arg(i));
            message->setIcon(QMessageBox::Icon::Critical);
            return message;
        }
        i++;
    }
    return nullptr;
}

void WindGeneratorTab::loadDataFromFile(const QString path)
{
    QFile file(path);
    if( !file.open( QIODevice::ReadOnly ) )
    {
        //TODO
         return;
    }

    QByteArray data = file.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    QJsonObject objectWG1 = document.object()["WG1"].toObject();
    ui->mSelectWG_1->activated(objectWG1["index"].toInt());

    QJsonObject objectWG2 = document.object()["WG2"].toObject();
    ui->mSelectWG_2->activated(objectWG1["index"].toInt());


}

void WindGeneratorTab::saveInFile(QJsonObject & root)
{
    QVariantMap wg1, wg2;
    wg1.insert("index", ui->mSelectWG_1->currentIndex());
    wg2.insert("index", ui->mSelectWG_2->currentIndex());
    root.insert("WG1", QJsonObject::fromVariantMap(wg1));
    root.insert("WG2", QJsonObject::fromVariantMap(wg2));
}

void WindGeneratorTab::sumPowerWG()
{
    double sumPower;
    auto proxy1 = qobject_cast<WGProxyModel*>(ui->tableView_WG_1->model());
    sumPower += proxy1->getDataWG().nomPower;
    auto proxy2 = qobject_cast<WGProxyModel*>(ui->tableView_WG_2->model());
    sumPower += proxy2->getDataWG().nomPower;
    emit changePower(proxy1->getDataWG().nomPower / 1000, LineTab::LINE_WG1);
    emit changePower(proxy2->getDataWG().nomPower / 1000, LineTab::LINE_WG2);
    emit changePower(sumPower);
}

void WindGeneratorTab::changeMarkWG1(int index)
{
    ui->CheckIcon_1->setPixmap(index == 0 ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    mValidWG[0] = index != 0;
    sumPowerWG();
    ui->speedPowerButton1->setEnabled(index == 1);
    emit changeModelValidIcon(TabIndices::WG1, index != 0);
}

void WindGeneratorTab::changeMarkWG2(int index)
{
    ui->CheckIcon_2->setPixmap(index == 0 ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    mValidWG[1] = index != 0;
    sumPowerWG();
    ui->speedPowerButton2->setEnabled(index == 1);
    emit changeModelValidIcon(TabIndices::WG2, index != 0);
}

void WindGeneratorTab::setSpeedPowerWG2()
{
    OnSpeedPowerWindow(SpeedPowerModel::WG2);
}

void WindGeneratorTab::setSpeedPowerWG1()
{
    OnSpeedPowerWindow(SpeedPowerModel::WG1);
}

void WindGeneratorTab::OnSpeedPowerWindow(SpeedPowerModel::Dependence wg)
{
    mSpeedPowerWindow.setDependence(wg);
    mSpeedPowerWindow.setTitleText(ui->mSelectedWG->text());
    mSpeedPowerWindow.show();
}
