#ifndef VALIDATABLELINEEDIT_H
#define VALIDATABLELINEEDIT_H

#include <QObject>
#include <QLineEdit>

class ValidatableLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    ValidatableLineEdit(QWidget * parent = nullptr);
};

#endif // VALIDATABLELINEEDIT_H
