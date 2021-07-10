#include <QLineEdit>
#include <QItemDelegate>

#include "CoefficientValidator.h"


CoefficientValidator::CoefficientValidator(QObject *parent, double bottom, double top):QItemDelegate (parent), mValidator(bottom, top, this),mBottom(bottom),mTop(top)
{

}

QWidget *CoefficientValidator::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(&mValidator);
    return editor;
}

void CoefficientValidator::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    line->setText(value);
}

void CoefficientValidator::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    model->setData(index, value);
}

void CoefficientValidator::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
