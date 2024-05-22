#include "mainwindow.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    Client::getInstance();
    return a.exec();
}
