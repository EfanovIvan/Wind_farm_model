#ifndef SHAREDUTILS_H
#define SHAREDUTILS_H
#include <QVector>
class QCustomPlot;

namespace SharedUtils
{
    void drawGraph(QCustomPlot *customPlot, int Power, QVector<double> k, int typeGraph);
    void initGraph(QCustomPlot *customPlot);

};

#endif // SHAREDUTILS_H
