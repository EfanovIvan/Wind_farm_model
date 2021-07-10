#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H

#include <QObject>
#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    DoubleValidator(double bottom, double top, QObject * parent = nullptr);
    QDoubleValidator::State validate(QString &str, int &pos) const override;
    double mBottom;
    double mTop;
};

#endif // DOUBLEVALIDATOR_H
