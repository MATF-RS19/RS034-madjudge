#include "mainwindow.h"
#include <QApplication>
#include "user.h"
#include "popuplogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    User::Init();
    PopupLogin pl;
    pl.show();

    MainWindow w;

   // w.show();

    User::Get()->Connect();

    return a.exec();
}
