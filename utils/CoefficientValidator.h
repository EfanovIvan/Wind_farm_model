#ifndef COEFFICIENTVALIDATOR_H
#define COEFFICIENTVALIDATOR_H


#include <QItemDelegate>
#include "DoubleValidator.h"

class CoefficientValidator : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CoefficientValidator(QObject *paren, double bottom, double top);

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;

private:
    double mBottom;
    double mTop;
    DoubleValidator mValidator;
};

#endif // COEFFICIENTVALIDATOR_H
