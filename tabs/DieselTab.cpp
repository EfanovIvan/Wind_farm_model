
#include <QFile>
#include <QTextCodec>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "utils/CoefficientValidator.h"
#include "tabs/DieselTab.h"
#include "ui_diesel.h"
#include "models/DieselModel.h"
#include "models/ProxySelectGeneratorModel.h"
#include "models/WGProxyModel.h"

DieselTab::DieselTab(QWidget *parent) : QWidget(parent), ui(new Ui::Diesel),mValidIcon(false)
{
    ui->setupUi(this);

    DieselModel * diesel = new DieselModel(this);
    ui->dieselView->setModel(new WGProxyModel(diesel, this, WGProxyModel::TypeGenerators::Diesel));
    auto validator = new CoefficientValidator(this, 0.00, 1000.0);
     ui->dieselView->setItemDelegate(validator);
    ui->SelectDiesel->setModel(new ProxySelectGeneratorModel(diesel, 11, 9, this));
    ui->dieselView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dieselView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dieselView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    QObject::connect(ui->SelectDiesel, SIGNAL(activated(int)), ui->dieselView->model(), SLOT(chageIndexColumn(int)));
    QObject::connect(ui->SelectDiesel, SIGNAL(activated(int)), ui->SelectDiesel->model(), SLOT(changeIndexImage(int)));
    QObject::connect(ui->SelectDiesel->model(), SIGNAL(changeImage(QPixmap )), ui->DieselImage, SLOT(setPixmap(QPixmap)));
    QObject::connect(ui->SelectDiesel, SIGNAL(activated(int)), this, SLOT(changeMarkDG(int)));
}

DieselTab::~DieselTab()
{
    delete ui;
}

WGProxyModel::DataWG DieselTab::getDataDiesel()
{
    auto proxy = qobject_cast<WGProxyModel*>(ui->dieselView->model());
    return proxy->getDataWG();
}

void DieselTab::loadDataFromFile(const QString path)
{
    QFile file(path);
    if( !file.open( QIODevice::ReadOnly ) )
    {
//        messageBox = std::make_shared<QMessageBox>();
//        messageBox->setText(QString("Невозможно открыть файл"));
//        messageBox->show();
    }

    QByteArray data = file.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(data);
    QJsonDocument document;
    document = document.fromJson( unicode.toUtf8() );

    QJsonObject objectWG1 = document.object()["Diesel"].toObject();
    ui->SelectDiesel->activated(objectWG1["index"].toInt());

}

void DieselTab::saveInFile(QJsonObject &root)
{
    QVariantMap diesel;
    diesel.insert("index", ui->SelectDiesel->currentIndex());
    root.insert("Diesel", QJsonObject::fromVariantMap(diesel));
}

void DieselTab::changeMarkDG(int index)
{
    ui->mCheckIcon_3->setPixmap(index == 0 ? QPixmap(":/Resources/Icon/warning4.png") : QPixmap(":/Resources/Icon/accept2.png"));
    mValidIcon = index == 0;
    emit changeModelValidIcon(TabIndices::Diesel, index != 0);
}
