#include <QtCore/QCoreApplication>
#include "server.h"
#include "contest/contestmanager.h"
#include "contest/problemsetmanager.h"

int main( int argc, char** argv )
{
    QCoreApplication a( argc, argv );

    if (!ProblemSetManager::Init("problems"))
    {
        exit(EXIT_FAILURE);
    }
    ContestManager::Init();
    qDebug() << "Manager initialization complete.";

    return a.exec();
}
