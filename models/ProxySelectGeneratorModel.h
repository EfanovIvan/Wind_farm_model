#ifndef WGPROXYSPRINBOXMODEL_H
#define WGPROXYSPRINBOXMODEL_H

#include <QIdentityProxyModel>
#include <QObject>
#include <QPixmap>

class ProxySelectGeneratorModel  : public QIdentityProxyModel
{
    Q_OBJECT
public:
    ProxySelectGeneratorModel(QAbstractItemModel *sourceModel, int indexImage, int indexText, QObject *parent = 0);
public slots:
    void changeIndexImage(int index);
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void setSizeLagel( const QSize & size) { sizeLabel = size;}

    // QAbstractProxyModel interface
signals:
    void changeImage(QPixmap );
    void changeWarningIcon(QPixmap );

private:
    QSize sizeLabel;
    int mIndexImage;
    int mIndexText;

    // QAbstractItemModel interface
public:
    bool selected() {return mIndexText > 0; }
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // WGPROXYSPRINBOXMODEL_H
