#include "InfoWindow.h"
#include "ui_infiwindow.h"

InfoWindow::InfoWindow(QDialog *parent) : QDialog(parent), ui(new Ui::Info)
{
    ui->setupUi(this);
    setWindowTitle("Info");
}
