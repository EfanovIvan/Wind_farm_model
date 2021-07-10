#ifndef CELLWINDOWWITHOUTCONTACTOR_H
#define CELLWINDOWWITHOUTCONTACTOR_H

#include <QDialog>
#include <QObject>

#include "CellWindow.h"
namespace Ui {
class Disconnector;
}

class CellWindowWithoutContactor : public QDialog
{
    Q_OBJECT
public:
    explicit CellWindowWithoutContactor(QDialog *parent = nullptr);
    ~CellWindowWithoutContactor();

signals:
    void changStateDisconnector(int, CellWindow::DataCell);
public slots:

    void clicked(int);
    void updateViewCell();
    void setIndexCell(int index);
    QVector<CellWindow::DataCell> getCellData(TabIndices index);
    void setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData);
private:
    Ui::Disconnector * ui;
    int             mCurrentIndex;
    QMap<int, CellWindow::DataCell> mCellsData;
};

#endif // CELLWINDOWWITHOUTCONTACTOR_H
