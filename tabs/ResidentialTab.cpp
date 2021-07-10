
#include <QFile>
#include <QTextCodec>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QLineEdit>

#include "tabs/ResidentialTab.h"
#include "ui_residential.h"
#include "models/TableCoefficientsPower.h"
#include "models/ProxyCoefficientPower.h"
#include "utils/SharedUtils.h"
#include "models/ProxyLineModel.h"
#include "tabs/TabIndex.h"
#include "utils/CoefficientValidator.h"
#include "tabs/LineTab.h"

ResidentialTab::ResidentialTab(QWidget *parent) : QWidget(parent), ui(new Ui::Residential)
{
    ui->setupUi(this);

    QObject::connect(ui->powerOneHouse, SIGNAL(editingFinished()), this, SLOT(recountPower()));
    QObject::connect(ui->powerAdminHouse, SIGNAL(editingFinished()), this, SLOT(recountPower()));
    QObject::connect(ui->countHouses, SIGNAL(editingFinished()), this, SLOT(recountPower()));
    mCoefficients[ProxyCoefficientPower::SUMMER].fill(1.0, 24);
    mCoefficients[ProxyCoefficientPower::WINTER].fill(1.0, 24);
    auto validator = new QDoubleValidator(0, 100, 2, this );
    validator->setNotation(QDoubleValidator::StandardNotation);
    ui->powerOneHouse->setValidator(validator);
    ui->countHouses->setValidator(new QIntValidator(0, 100, this ));
    ui->powerAdminHouse->setValidator(validator);
}

ResidentialTab::~ResidentialTab()
{
    delete ui;
}

void ResidentialTab::setModel(TableCoefficientsPower * tableCoefficients)
{
    auto validator = new CoefficientValidator(this, 0.00, 1.0);
    ui->tableCoefficient_2->setItemDelegate(validator);
    ui->tableCoefficient_2->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableCoefficient_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableCoefficient_2->setModel(new ProxyCoefficientPower(tableCoefficients, 1, this));
    ui->tableCoefficient_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableCoefficient_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    SharedUtils::initGraph(ui->Graphic_2);
    QObject::connect(ui->tableCoefficient_2->model(), SIGNAL(drawIndustrialGraph(QVector<double>, int)), this, SLOT(drawGraph(QVector<double>, int)));
}

void ResidentialTab::drawGraph(QVector<double> k, int type)
{
   double power = getPower();
   SharedUtils::drawGraph(ui->Graphic_2, power, k, type);
   for(int i = 0; i < k.size(); i++)
   {
       mCoefficients[type][i] = k[i];
   }
}

void ResidentialTab::recountPower()
{
    double power = getPower();
    emit updatePower(TabIndices::Line1, getPower());
    emit changePower(power, LineTab::LINE_1_RESIDENTIAL);
    SharedUtils::drawGraph(ui->Graphic_2, power, mCoefficients[ProxyCoefficientPower::WINTER], 0);
    SharedUtils::drawGraph(ui->Graphic_2, power, mCoefficients[ProxyCoefficientPower::SUMMER], 0);
    ui->powerOneHouse->clearFocus();
    ui->powerAdminHouse->clearFocus();
    ui->countHouses->clearFocus();
}

double ResidentialTab::getPower(int currentMonth, int currentHour)
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

double ResidentialTab::getPower()
{
    QString powerOneHouse(ui->powerOneHouse->text());
    powerOneHouse.replace(",", ".");

    QString powerAdminHouse(ui->powerAdminHouse->text());
    powerAdminHouse.replace(",", ".");

    auto countHouses = ui->countHouses->text();
    double power = (powerOneHouse.toDouble() * countHouses.toInt()) + powerAdminHouse.toDouble();
    return power;
}

void ResidentialTab::loadDataFromFile(const QString path)
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


    QJsonObject ResidentialTab = document.object()["ResidentialTab"].toObject();
    ui->powerOneHouse->setText(ResidentialTab["house"].toString());
    ui->countHouses->setText(ResidentialTab["count"].toString());
    ui->powerAdminHouse->setText(ResidentialTab["adminHouse"].toString());

    for(int i = 0; i < 2; i++)
    {
        QJsonValue agentsArrayValue = ResidentialTab.value(i == 0 ? "Summer": "Winter");
        QJsonArray agentsArray = agentsArrayValue.toArray();

        if(auto model = qobject_cast<ProxyCoefficientPower*>(ui->tableCoefficient_2->model()))
        {
            int index = 0;
            foreach (const QJsonValue & v, agentsArray)
            {
                model->setData(model->index(i, index), v.toObject()[QString::number(index)].toString().toDouble(), Qt::EditRole);
                index++;
            }

        }
    }
    emit updatePower(TabIndices::Line1, getPower());
    emit changePower(getPower(), LineTab::LINE_1_RESIDENTIAL);

}

void ResidentialTab::saveInFile(QJsonObject & root)
{
    QVariantMap residentialTab;
    QVariantMap dataMap;

    dataMap.insert("house", ui->powerOneHouse->text());
    dataMap.insert("count", ui->countHouses->text());
    dataMap.insert("adminHouse", ui->powerAdminHouse->text());

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

    root.insert("ResidentialTab", QJsonObject::fromVariantMap(dataMap));
}

