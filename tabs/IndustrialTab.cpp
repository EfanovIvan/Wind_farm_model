#include "ui_industrial.h"
#include "tabs/IndustrialTab.h"
#include "models/IndustrialModel.h"
#include "models/ProxyIndustrialModel.h"
#include "models/TableCoefficientsPower.h"
#include "models/ProxyCoefficientPower.h"
#include "models/ProxyLineModel.h"
#include "utils/SharedUtils.h"
#include "tabs/TabIndex.h"
#include "utils/CoefficientValidator.h"
#include "tabs/LineTab.h"

extern int GLOBAL_CURRENT_HOUER;

IndustrialTab::IndustrialTab(QWidget *parent) : QWidget(parent), ui(new Ui::Industrial),tableCoeff()
{
    ui->setupUi(this);

    IndustrialModel * industrialModel = new IndustrialModel(this);
    ui->tableViewGarage->setModel(new ProxyIndustrialModel(industrialModel, 0, ProxyIndustrialModel::GARAGE, this));
    QObject::connect(ui->tableViewGarage->model(), SIGNAL(sumGarage(QString )), ui->SumGarage, SLOT(setText(QString)));
    ui->tableViewGarage->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGarage->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGarage->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableViewGarage->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");

    ui->tableViewManufactory->setModel(new ProxyIndustrialModel(industrialModel, 7, ProxyIndustrialModel::MANUFACTORY, this));
    QObject::connect(ui->tableViewManufactory->model(), SIGNAL(sumGarage(QString )), ui->SumManufactory, SLOT(setText(QString)));
    ui->tableViewManufactory->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewManufactory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewManufactory->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableViewManufactory->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");

    ui->tableViewGreenHouse->setModel(new ProxyIndustrialModel(industrialModel, 14, ProxyIndustrialModel::GREENHOUSE, this));
    QObject::connect(ui->tableViewGreenHouse->model(), SIGNAL(sumGarage(QString )), ui->SumGreenHouse, SLOT(setText(QString)));
    ui->tableViewGreenHouse->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGreenHouse->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGreenHouse->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableViewGreenHouse->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");

    auto validator = new CoefficientValidator(this, 0.00, 100.0);
    ui->tableViewGreenHouse->setItemDelegate(validator);
    ui->tableViewGarage->setItemDelegate(validator);
    ui->tableViewManufactory->setItemDelegate(validator);

    QObject::connect(ui->tableViewGreenHouse->model(), SIGNAL(changePower(double, int)), this, SLOT(recountPower(double, int)));
    QObject::connect(ui->tableViewGarage->model(), SIGNAL(changePower(double, int)), this, SLOT(recountPower(double, int)));
    QObject::connect(ui->tableViewManufactory->model(), SIGNAL(changePower(double, int)), this, SLOT(recountPower(double, int)));

    mCoefficients[ProxyCoefficientPower::SUMMER].fill(1.0, 24);
    mCoefficients[ProxyCoefficientPower::WINTER].fill(1.0, 24);
}

IndustrialTab::~IndustrialTab()
{
    delete ui;
}

void IndustrialTab::setModel(TableCoefficientsPower * tableCoefficients)
{
    auto validator = new CoefficientValidator(this, 0.00, 1.0);
    ui->tablePowerCoefficient->setItemDelegate(validator);
    ui->tablePowerCoefficient->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tablePowerCoefficient->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tablePowerCoefficient->setModel(new ProxyCoefficientPower(tableCoefficients, 0, this));
    ui->tablePowerCoefficient->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablePowerCoefficient->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->tablePowerCoefficient->model(), SIGNAL(drawIndustrialGraph(QVector<double> , int)), this, SLOT(drawGraph(QVector<double> , int)));
    SharedUtils::initGraph(ui->Graphic_1);
}

void IndustrialTab::drawGraph(QVector<double>  k, int type)
{
    auto sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewGarage->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewManufactory->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewGreenHouse->model())->getSupPower();
    SharedUtils::drawGraph(ui->Graphic_1, sum, k, type);
    for(int i = 0; i < k.size(); i++)
    {
        mCoefficients[type][i] = k[i];
    }
}

void IndustrialTab::recountPower(double power, int type)
{
    double sum = 0.0;
    if(type == ProxyIndustrialModel::TypeIndustrialStructure::GARAGE)
    {
        sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewManufactory->model())->getSupPower();
        sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewGreenHouse->model())->getSupPower();
        sum += power;
    }
    else if(type == ProxyIndustrialModel::TypeIndustrialStructure::GREENHOUSE)
    {
        sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewGarage->model())->getSupPower();
        sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewManufactory->model())->getSupPower();
        sum += power;
    }
    else if(type == ProxyIndustrialModel::TypeIndustrialStructure::MANUFACTORY)
    {
        sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewGarage->model())->getSupPower();
        sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewGreenHouse->model())->getSupPower();
        sum += power;
    }
    emit changePower(sum, LineTab::LINE_3_INDUSTRIAL);
    emit updatePower(TabIndices::Line3, getPower());
    SharedUtils::drawGraph(ui->Graphic_1, sum, mCoefficients[ProxyCoefficientPower::WINTER], 0);
    SharedUtils::drawGraph(ui->Graphic_1, sum, mCoefficients[ProxyCoefficientPower::SUMMER], 0);
}

double IndustrialTab::getPower(int currentMonth, int currentHour)
{
    auto sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewGarage->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewManufactory->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewGreenHouse->model())->getSupPower();
    if(currentMonth == 0 || currentMonth == 1 || currentMonth == 11)
    {
        return sum * mCoefficients[ProxyCoefficientPower::WINTER][currentHour];
    }
    else
    {
        return sum * mCoefficients[ProxyCoefficientPower::SUMMER][currentHour];
    }
}

double IndustrialTab::getPower()
{
    auto sum = qobject_cast<ProxyIndustrialModel*>(ui->tableViewGarage->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewManufactory->model())->getSupPower();
    sum += qobject_cast<ProxyIndustrialModel*>(ui->tableViewGreenHouse->model())->getSupPower();
    return sum;
}

void IndustrialTab::loadDataFromFile(const QString path)
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


    QJsonObject ResidentialTab = document.object()["IndustrialTab"].toObject();


    for(int i = 0; i < 2; i++)
    {
        QJsonValue agentsArrayValue = ResidentialTab.value(i == 0 ? "Summer": "Winter");
        QJsonArray agentsArray = agentsArrayValue.toArray();

        if(auto model = qobject_cast<ProxyCoefficientPower*>(ui->tablePowerCoefficient->model()))
        {
            int index = 0;
            foreach (const QJsonValue & v, agentsArray)
            {
                model->setData(model->index(i, index), v.toObject()[QString::number(index)].toString().toDouble(), Qt::EditRole);
                index++;
            }

        }
    }
   // emit updatePower(TabIndices::Line1, getPower());
   // emit changePower(getPower(), LineTab::LINE_1_RESIDENTIAL);

}

void IndustrialTab::saveInFile(QJsonObject &root)
{
    QVariantMap residentialTab;
    QVariantMap dataMap;


    std::array<QJsonArray,2> summerAndWinter;

    for(int i = 0; i < 2; i++)
    {
        for(int hour = 0; hour < mCoefficients[i].size(); hour++)
        {
            QJsonObject item_data;
            item_data.insert(QString::number(hour), QString::number(mCoefficients[i][hour], 'f', 1));
            summerAndWinter[i].push_back(item_data);
        }
        dataMap.insert(i == 0 ? "Summer":"Winter", summerAndWinter[i]);
    }

    root.insert("IndustrialTab", QJsonObject::fromVariantMap(dataMap));
}
