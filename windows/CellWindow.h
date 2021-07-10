#ifndef CELLWINDOW_H
#define CELLWINDOW_H

#include <QObject>
#include <QDialog>
#include <QMap>
#include <QMessageBox>
#include <memory>
#include <QtCore>

#include "tabs/TabIndex.h"

namespace Ui {
class CellDialog;
}

class CellWindow : public QDialog
{
    Q_OBJECT
public:
    explicit CellWindow(QDialog  *parent = nullptr);
    ~CellWindow();
    void setIndexCell(int index);
    struct DataCell
    {
        DataCell():onContactor(false),onDisconnector(true),automatic(true){}
        bool onContactor;
        bool onDisconnector;
        bool automatic;
    };
    void updateAllElement();
     QVector<CellWindow::DataCell> getCellData(TabIndices index);
     void setCellData(TabIndices index, QVector<CellWindow::DataCell> cellData);
signals:
    void changStateDisconnector(int, CellWindow::DataCell);
public slots:

    void clicked(int);
    void updateViewCell();
    void onAutoControl();
    void onManualControl();
private:
    std::shared_ptr<QMessageBox> messageBox;
    Ui::CellDialog * ui;
    int             mCurrentIndex;
    QMap<int, CellWindow::DataCell> mCellsData;
};
Q_DECLARE_METATYPE(CellWindow::DataCell);

#endif // CELLWINDOW_H
