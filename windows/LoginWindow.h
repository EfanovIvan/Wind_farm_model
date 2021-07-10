#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QDialog>

namespace Ui {
    class Login;
}
class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    enum TypeAuthorization
    {
        NONE,
        SAVE,
        LOAD
    };
    LoginWindow(QDialog * parent = nullptr);
    bool getAuthorization() { return true;}//authorization;}
    void setAuthorizationType(TypeAuthorization type);
signals:
    void closeAuthorizationWindow(int);
public slots:
    void checkUser();
    void onClose();
private:
    Ui::Login * ui;
    bool authorization;
    TypeAuthorization typeAuthorization;
};

#endif // LOGINWINDOW_H
