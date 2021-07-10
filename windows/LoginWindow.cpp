#include <QDate>
#include <QTime>
#include <QString>
#include <QCloseEvent>

#include "LoginWindow.h"
#include "ui_authorization.h"

LoginWindow::LoginWindow(QDialog * parent): QDialog (parent), ui(new Ui::Login),authorization(false),typeAuthorization(NONE)
{
    ui->setupUi(this);
    QObject::connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onClose()));
    QObject::connect(ui->okButton, SIGNAL(clicked()), this, SLOT(checkUser()));
    setWindowTitle("Загрузка и сохранение проекта");
}

void LoginWindow::setAuthorizationType(LoginWindow::TypeAuthorization type)
{
    typeAuthorization = type;
}

void LoginWindow::checkUser()
{
    close();
    emit closeAuthorizationWindow(typeAuthorization);
}

void LoginWindow::onClose()
{
    close();
}



