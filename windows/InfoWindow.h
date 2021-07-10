#ifndef INFOWINFOW_H
#define INFOWINFOW_H

#include <QDialog>

namespace Ui {
class Info;
}
class InfoWindow : public QDialog
{
    Q_OBJECT
public:
    explicit InfoWindow(QDialog *parent = nullptr);

signals:

public slots:

private:
    Ui::Info * ui;
};

#endif // INFOWINFOW_H
