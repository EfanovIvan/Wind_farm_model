#include "SharedUtils.h"
#include "qcustomplot.h"
#include <QVector>
#include <QPen>
#include <QColor>
namespace SharedUtils
{
    void drawGraph(QCustomPlot *customPlot, int Power, QVector<double> k, int typeGraph)
    {
        QVector<double> x(25), y(25);
        for (int i=0; i<25; i++)
        {
            if(i == 24)
            {
                x[i] = i;
                continue;
            }

          x[i] = i;
          y[i] = Power * k[i];
        }

        customPlot->graph(typeGraph)->setData(x, y);
        customPlot->yAxis->setRange(0, Power);
        customPlot->replot();
    }

    void initGraph(QCustomPlot *customPlot)
    {
        QVector<double> ticks;
        QVector<QString> labels;
        QStringList lineNames;
        lineNames << "лето" << "зима";
        for(int i = 1; i <=24; i++)
        {
            labels.append(std::to_string(i).c_str());
            ticks.append(i);
        }

        customPlot->legend->setVisible(true);
        QPen pen;
        for(int i = 0; i < 2; i++)
        {
            customPlot->addGraph();
            pen.setColor(i == 0 ? Qt::red : Qt::blue);
            customPlot->graph()->setPen(pen);
            customPlot->graph()->setName(lineNames.at(i));
            customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsStepLeft);
            customPlot->xAxis->setAutoTicks(false);
            customPlot->xAxis->setAutoTickLabels(false);
            customPlot->xAxis->setTickVector(ticks);
            customPlot->xAxis->setTickVectorLabels(labels);
            customPlot->xAxis->setSubTickCount(0);


            customPlot->xAxis->grid()->setVisible(true);

            customPlot->xAxis->setLabel("t, ч");
            customPlot->yAxis->setLabel("P, кВт");
            customPlot->xAxis->setRange(0, 24);
            customPlot->yAxis->setRange(0, 5000);
            customPlot->replot();
        }

    }
}


