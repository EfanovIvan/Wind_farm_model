
#include <array>
#include <QFile>
#include <QTextCodec>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>
#include <QObject>
#include <QAbstractItemModel>
#include <QSpinBox>

#include "qcustomplot.h"
#include "tabs/ResultTab.h"
#include "ui_result.h"
#include "models/ResultModel.h"


ResultTab::ResultTab(QWidget *parent) : QWidget(parent), ui(new Ui::Result)
{
    ui->setupUi(this);
    ui->tableView->setModel(new ResultModel(this));
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: #B0E0E6;}");
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->exelButton, SIGNAL(clicked()), this , SLOT(expotExel()));
    QObject::connect(ui->tableView->model(), SIGNAL(sumWGEnergy(double)), this , SLOT(setSumWGEnergy(double)));
    QObject::connect(ui->tableView->model(), SIGNAL(sumDieselEnergy(double)), this , SLOT(setSumDiselEnergy(double )));
    QObject::connect(ui->tableView->model(), SIGNAL(totalSum(double)), this , SLOT(setTotalSumEnergy(double )));
     auto sum = QChar(0xA3, 0x03);
    ui->totalSumEnergy->setText(QString("%1Wобщ = %2").arg(sum).arg(0.0));
    ui->Wwg->setText(QString("%1Wвэу = %2").arg(sum).arg(0.0));
    ui->Wdiesel->setText(QString("%1Wдэг = %2").arg(sum).arg(0.0));
    initGraph();
}

ResultTab::~ResultTab()
{
    delete ui;
}

std::shared_ptr<QMessageBox> ResultTab::check()
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
         if(!model->beaufortBallSet())
         {
             auto message = std::make_shared<QMessageBox>();
             message->setText(QString("Нужно нужно внести в таблицу баллы Бафорта"));
             message->setIcon(QMessageBox::Icon::Critical);
             return message;
         }
    }
    return nullptr;
}

void ResultTab::setCalculationData(const std::array<QVector<QVariant>,3> & calculateData)
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        model->setCalculateData(calculateData);
        std::array<QVector<QVariant>,3> temp = std::move(calculateData);
        for(int j = 0; j < 3; j++)
        {
            for(int i = 0; i < 12; i++)
            {
               temp[j][i] = temp[j][i].toDouble() * ResultModel::dataInMonth[i] * 24;
            }
        }

         double maxWG = 0.0;
         double maxDiesel = 0.0;
       // ResultModel::dataInMonth
        QVector<double> x(12 ), y(12);
        for(int j = 0; j < 2; j++)
        {
            for (int i=0; i<12; i++)
            {
              x[i] = i + 1;
              y[i] = temp[j][i].toDouble();
            }

            if(j == 0)
            {
                maxWG = std::max_element(temp[j].begin(), temp[j].end())->toDouble();
            }
            else if(j == 1)
            {
                 maxDiesel = std::max_element(temp[j].begin(), temp[j].end())->toDouble();
            }
            ui->Graphic_1->graph(j)->setData(x, y);
            //double max = std::max_element(temp[j].begin(), temp[j].end())->toDouble();
            //std::max_element(temp[j].begin(), temp[j].end())->toDouble());
        }
        ui->Graphic_1->yAxis->setRange(0, std::max<double>(maxWG, maxDiesel));

        ui->Graphic_1->replot();
    }
}

void ResultTab::clearData()
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        model->clearData();
    }
}

QPair<double, double> ResultTab::getBeaufortRange(int numberOfDay)
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        return model->getBeaufortRange(numberOfDay);
    }
}

void ResultTab::loadDataFromFile(const QString path)
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

    QJsonObject ResidentialTab = document.object()["ResultTab"].toObject();

    QJsonValue agentsArrayValue = ResidentialTab.value("BeaufortBalls");
    QJsonArray agentsArray = agentsArrayValue.toArray();

    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        int index = 0;
        foreach (const QJsonValue & v, agentsArray)
        {
            model->setData(model->index(index, 1), v.toObject()["ball"].toInt(), Qt::EditRole);
            index++;
        }

    }
}

void ResultTab::saveInFile(QJsonObject & root)
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        auto balls = model->getBeaufortBall();

        QVariantMap resultTab;
        QVariantMap ball;
        QJsonArray beaufortBalls;

        for(int i = 0; i < balls.size(); i++)
        {
            QJsonObject item_data;
            item_data.insert("ball", balls[i]);
            beaufortBalls.push_back(item_data);
        }
        ball.insert("BeaufortBalls", beaufortBalls);
        root.insert("ResultTab", QJsonObject::fromVariantMap(ball));
    }

}

void ResultTab::expotExel()
{
    if(auto model = qobject_cast<ResultModel*>(ui->tableView->model()))
    {
        model->expotExel();
    }
}

void ResultTab::setSumWGEnergy(double energy)
{
    auto sum = QChar(0xA3, 0x03);
    ui->Wwg->setText(QString("%1Wвэу_акб = %2").arg(sum).arg(QString::number(energy, 'f',2)));
}

void ResultTab::setSumDiselEnergy(double energy)
{
    auto sum = QChar(0xA3, 0x03);
    ui->Wdiesel->setText(QString("%1Wдэг = %2").arg(sum).arg(QString::number(energy, 'f',2)));
}

void ResultTab::setTotalSumEnergy(double totalSumEnergy)
{
    auto sum = QChar(0xA3, 0x03);
    ui->totalSumEnergy->setText(QString("%1Wобщ = %2").arg(sum).arg(QString::number(totalSumEnergy, 'f', 2)));
}

void ResultTab::initGraph()
{
    QVector<double> ticks;
    QVector<QString> labels;
    QStringList lineNames;
    lineNames << "Wвэу" << "Wдэг";
    for(int i = 1; i <=12; i++)
    {
        labels.append(std::to_string(i).c_str());
        ticks.append(i);
    }

    ui->Graphic_1->legend->setVisible(true);
    QPen pen;
    for(int i = 0; i < 2; i++)
    {
        ui->Graphic_1->addGraph();
        pen.setColor(i == 0 ? Qt::red : Qt::blue);
        ui->Graphic_1->graph()->setPen(pen);
        ui->Graphic_1->graph()->setName(lineNames.at(i));
        ui->Graphic_1->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);
        ui->Graphic_1->xAxis->setAutoTicks(false);
        ui->Graphic_1->xAxis->setAutoTickLabels(false);
        ui->Graphic_1->xAxis->setTickVector(ticks);
        ui->Graphic_1->xAxis->setTickVectorLabels(labels);
        ui->Graphic_1->xAxis->setSubTickCount(0);


        ui->Graphic_1->xAxis->setTickVector(ticks);
        ui->Graphic_1->xAxis->setTickVectorLabels(labels);
        ui->Graphic_1->xAxis->setSubTickCount(0);


        ui->Graphic_1->xAxis->grid()->setVisible(true);

        ui->Graphic_1->xAxis->setLabel("t, месяц");
        ui->Graphic_1->yAxis->setLabel("W, МВт * ч");
        ui->Graphic_1->xAxis->setRange(1, 12);
        ui->Graphic_1->yAxis->setRange(0, 30000);
        ui->Graphic_1->replot();
    }
}
