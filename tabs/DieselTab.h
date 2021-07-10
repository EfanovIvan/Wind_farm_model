#ifndef DIESELTAB_H
#define DIESELTAB_H
#include <QWidget>

#include "models/WGProxyModel.h"
#include "utils/LoadDataFromFile.h"
#include "tabs/TabIndex.h"

namespace Ui {
    class Diesel;
}


class DieselTab : public QWidget, public LoadDataFromFile
{
    Q_OBJECT
public:
    explicit DieselTab(QWidget *parent = nullptr);
    ~DieselTab() override;
    WGProxyModel::DataWG getDataDiesel();
    void loadDataFromFile(const QString path) override;
    void saveInFile(QJsonObject & root) override;
signals:
    void changeModelValidIcon(TabIndices, bool);
public slots:
    void changeMarkDG(int);
private:
    Ui::Diesel * ui;
    bool mValidIcon;
};

#endif // DIESELTAB_H
