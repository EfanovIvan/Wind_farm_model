#ifndef FUSEMODEL_H
#define FUSEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
class FuseModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    FuseModel(QObject * parent = nullptr);
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QStringList stringKeys();
    QString getPathImage(int columm);
    QStringList getFuseList();
    QStringList getCurrentMelts(int index);
    QPixmap getImageFuse(int index);
    QPixmap getImageMelt(int index);
    int getCurrentFuse(int index);
    int getCurrentMelt(int indexKeyFuse, int indexMelt);
private:
    QVariant findKeyByColumn(QModelIndex index) const;
    void loadDataFromJson();
    QList<int> rowIndex;
    QMap<QString, QList<QVariant>>  mData;
    QHash<int, QString> mKeyString;
    QMap<QString, QList<QPair<QString, int>>> mMeltData;
};

#endif // FUSEMODEL_H
