#include "ValidatableLineEdit.h"
#include <QDoubleValidator>

ValidatableLineEdit::ValidatableLineEdit(QWidget * parent ): QLineEdit(parent)
{
    setValidator(new QDoubleValidator(0.00, 100.0, 2, this));
}
