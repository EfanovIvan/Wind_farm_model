#ifndef DISELMODEL_H
#define DISELMODEL_H

#include <QObject>
#include <QAbstractTableModel>
class DieselModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    DieselModel(QObject * parent = nullptr);

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QVariant findKeyByColumn(QModelIndex index) const;
    void loadDataFromJson();
    QMap<QString, QList<QVariant>>  mDataWG;
    QHash<int, QString>             mKeyString;
    static const QList<QString> textsParam;
};

#endif // DISELMODEL_H
