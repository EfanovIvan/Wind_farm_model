#ifndef DIGITALVALIDATOR_H
#define DIGITALVALIDATOR_H

#include <QValidator>

class DigitalValidator : public QValidator
{
public:
    DigitalValidator(QObject * parent = nullptr);

public:
    State validate(QString &, int &) const;
};

#endif // DIGITALVALIDATOR_H
