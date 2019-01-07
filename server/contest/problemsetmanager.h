#ifndef PROBLEMSETMANAGER_H
#define PROBLEMSETMANAGER_H

#include <QDir>
#include <QDirIterator>
#include <QVector>
#include <QDebug>
#include <QMutex>
#include <algorithm>
#include <random>
#include "../shared/problementry.h"

class ProblemSetManager
{

    static ProblemSetManager* h_Inst;                                               //singlton instanca

public:
    static bool Init(const QString& path);                                          //
    static void Shutdown();                                                         //
    static ProblemSetManager* Get();                                                //

    QMap<qint32, ProblemEntry *> NextRandomSet(qint32 count);                       //generiše se random set problema veličine count
private:
    ProblemSetManager(const QString& path);

    bool LoadFiles();                                                               //učitava probleme i njihova podešavanja iz fajlova, potrebno da se generiše problemset

    QVector<ProblemEntry*> m_problems;                                              //svi problemi
    QString m_path;                                                                 //put do direktorijuma gde se nalaze svi problemi
    qint32 m_counter;                                                               //brojač koji dodeljuje jedinstveni ID problemima

    QMutex m_problemsLock;                                                          //mutex koji zaključava funkciju nextrandomset kako se ne bi preplitale niti u njoj
};

#endif // PROBLEMSETMANAGER_H
