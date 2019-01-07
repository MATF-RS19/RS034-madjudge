#include "mainwindow.h"
#include <QApplication>
#include "user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    User::Init();
    MainWindow w;
    w.show();
    User::Get()->Connect();

    return a.exec();
}
