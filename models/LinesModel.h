#ifndef LINESMODE_H
#define LINESMODE_H
#include <QAbstractTableModel>
#include <QObject>
#include <QPair>
#include <memory>

class DigitalValidator;
class LinesMode : public QAbstractTableModel
{
    Q_OBJECT
public:
    LinesMode(QObject * parent = nullptr);

    struct DataWire
    {
        DataWire():name(""),Ilong(0),r(0.0),x(0.0),x0(0.0),r0(0.0){}
        QString name;
        int     Ilong;
        double   r;
        double   x;
        double   r0;
        double   x0;
    };

signals:
    void updateView();


public:
    static const QMap<int, QString> keyTexts;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, DataWire> mData;
    QHash<int, QString> mIndexKey;
    std::shared_ptr<DigitalValidator> validator;
    QString getKeyByIndex(int index) const { return mIndexKey.value(index); }
    void loadDataFromJson();
};

#endif // LINESMODE_H
