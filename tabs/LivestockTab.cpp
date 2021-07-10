#include <QString>

#include "tabs/LivestockTab.h"
#include "ui_livestock.h"
#include "utils/SharedUtils.h"
#include "models/TableCoefficientsPower.h"
#include "models/ProxyCoefficientPower.h"
#include "models/ProxyLineModel.h"
#include "tabs/TabIndex.h"
#include "utils/CoefficientValidator.h"

Livestock::Livestock(QWidget *parent) : QWidget(parent),ui(new Ui::Livestock)
{
    ui->setupUi(this);
    QObject::connect(ui->powerBirds, SIGNAL(editingFinished()), this, SLOT(recountPower()));
    QObject::connect(ui->powerLivestock, SIGNAL(editingFinished()), this, SLOT(recountPower()));
    mCoefficients[ProxyCoefficientPower::SUMMER].fill(1.0, 24);
    mCoefficients[ProxyCoefficientPower::WINTER].fill(1.0, 24);
}

Livestock::~Livestock()
{
    delete ui;
}

void Livestock::setModel(TableCoefficientsPower * tableCoefficients)
{
    auto validator = new CoefficientValidator(this, 0.00, 1.0);
    ui->tableCoefficient_3->setItemDelegate(validator);
    ui->tableCoefficient_3->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableCoefficient_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableCoefficient_3->setModel(new ProxyCoefficientPower(tableCoefficients, 2, this));
    ui->tableCoefficient_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableCoefficient_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    SharedUtils::initGraph(ui->Graphic_3);
    QObject::connect(ui->tableCoefficient_3->model(), SIGNAL(drawIndustrialGraph(QVector<double>, int)), this, SLOT(drawGraph(QVector<double>, int)));
}

void Livestock::drawGraph(QVector<double> k, int type)
{
    double power = getPower();
    SharedUtils::drawGraph(ui->Graphic_3, power, k, type);
    for(int i = 0; i < k.size(); i++)
    {
         mCoefficients[type][i] = k[i];
    }
}

void Livestock::recountPower()
{
    double power = getPower();
    emit changePower(power, LineTab::LINE_2_LIVESTOCK);
    emit updatePower(TabIndices::Line2, power);
    SharedUtils::drawGraph(ui->Graphic_3, power, mCoefficients[ProxyCoefficientPower::WINTER], 0);
    SharedUtils::drawGraph(ui->Graphic_3, power, mCoefficients[ProxyCoefficientPower::SUMMER], 0);
    ui->powerBirds->clearFocus();
    ui->powerLivestock->clearFocus();
}

double Livestock::getPower(int currentMonth, int currentHour)
{
    double power = getPower();
    if(currentMonth == 0 || currentMonth == 1 || currentMonth == 11)
    {
        return power * mCoefficients[ProxyCoefficientPower::WINTER][currentHour];
    }
    else
    {
        return power * mCoefficients[ProxyCoefficientPower::SUMMER][currentHour];
    }
}

double Livestock::getPower()
{
    QString powerBirds(ui->powerBirds->text());
    powerBirds.replace(",", ".");

    QString powerLivestock(ui->powerLivestock->text());
    powerLivestock.replace(",", ".");

    double power = powerBirds.toDouble() + powerLivestock.toDouble();
    return power;
}

void Livestock::loadDataFromFile(const QString path)
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


    QJsonObject ResidentialTab = document.object()["LivestockTab"].toObject();
    ui->powerBirds->setText(ResidentialTab["birds"].toString());
    ui->powerLivestock->setText(ResidentialTab["livestock"].toString());

    for(int i = 0; i < 2; i++)
    {
        QJsonValue agentsArrayValue = ResidentialTab.value(i == 0 ? "Summer": "Winter");
        QJsonArray agentsArray = agentsArrayValue.toArray();

        if(auto model = qobject_cast<ProxyCoefficientPower*>(ui->tableCoefficient_3->model()))
        {
            int index = 0;
            foreach (const QJsonValue & v, agentsArray)
            {
                model->setData(model->index(i, index), v.toObject()[QString::number(index)].toString().toDouble(), Qt::EditRole);
                index++;
            }

        }
    }

    double power = getPower();
    emit changePower(power, LineTab::LINE_2_LIVESTOCK);
    emit updatePower(TabIndices::Line2, power);
}

void Livestock::saveInFile(QJsonObject & root)
{
    QVariantMap residentialTab;
    QVariantMap dataMap;

    dataMap.insert("birds", ui->powerBirds->text());
    dataMap.insert("livestock", ui->powerLivestock->text());

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

    root.insert("LivestockTab", QJsonObject::fromVariantMap(dataMap));
}
