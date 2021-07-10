#ifndef DOUBLECELLWINDOW_H
#define DOUBLECELLWINDOW_H
#include <QObject>
#include <QDialog>
#include <QMap>
#include <QMessageBox>
#include <memory>
#include <QtCore>
#include <array>

#include "CellWindow.h"


namespace Ui {
class DoubleCell;
}

class DoubleCellWindow : public QDialog
{
    Q_OBJECT
public:
    DoubleCellWindow(QDialog * parent = nullptr);
    ~DoubleCellWindow();
    void setIndexCell(int index);
    void updateAllElement();
    QVector<CellWindow::DataCell> getCellData(TabIndices index);
    void setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData);
  signals:
      void changStateDisconnector(int, CellWindow::DataCell);
  public slots:

      void clicked(int);
      void clickedSecondCell(int index);
      void updateViewCell();
      void onAutoControl();
      void onManualControl();

  private:
      std::shared_ptr<QMessageBox> messageBox;
      Ui::DoubleCell * ui;
      int             mCurrentIndex;
      QMap<int, std::array<CellWindow::DataCell,2>> mCellsData;
};

#endif // DOUBLECELLWINDOW_H
