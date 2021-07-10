#include "DigitalMalidator.h"

DigitalValidator::DigitalValidator(QObject *parent):QValidator (parent)
{

}

QValidator::State DigitalValidator::validate(QString & str, int &) const
{
    if(str.size() == 0)
    {
        return Acceptable;
    }
    QRegExp rxp = QRegExp("^[0-9]+$");
    if(!str.contains(rxp))
    {
        return Invalid;
    }
    return Acceptable;

}
