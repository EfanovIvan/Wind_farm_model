#ifndef CHECKSELECTION_H
#define CHECKSELECTION_H
#include <memory>
#include <QObject>
class QMessageBox;


class CheckSelection
{
public:
    CheckSelection();
    virtual ~CheckSelection(){}
    virtual std::shared_ptr<QMessageBox> check() = 0;
};

#endif // CHECKSELECTION_H
