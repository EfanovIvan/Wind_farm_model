#include "windows/MainWindow.h"
#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QSize>
#include <iostream>
#include <QFile>

#define XLSX_NO_LIB

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Проект электроснабжения посёлка от ветро-дизельной установки");
    w.show();

    return a.exec();
}
