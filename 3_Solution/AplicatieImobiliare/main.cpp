#include "mainwindow.h"
#include "client.h"

#include "principalwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    Client::getInstance();

    // PrincipalWindow *pw=new PrincipalWindow(nullptr,"");
    // pw->show();

    return a.exec();
}
