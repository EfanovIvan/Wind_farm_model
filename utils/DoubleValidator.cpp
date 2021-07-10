#include "DoubleValidator.h"

DoubleValidator::DoubleValidator(double bottom, double top, QObject * parent):QDoubleValidator (),mBottom(bottom),mTop(top)
{
    setTop(mTop);
    setBottom(mBottom);
    setDecimals(2);
    setNotation(QDoubleValidator::StandardNotation);
}

QDoubleValidator::State DoubleValidator::validate(QString &str, int &pos) const
{
    str.replace(".", ",");
    return QDoubleValidator::validate(str, pos);
}
