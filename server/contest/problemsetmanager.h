#ifndef PROBLEMSETMANAGER_H
#define PROBLEMSETMANAGER_H

#include <QDir>

class ProblemSetManager
{

public:
    static void Init(const QString& path);
    static void Shutdown();

    ProblemSetManager* Get();
private:
    ProblemSetManager();

    static ProblemSetManager* h_Inst;

};

#endif // PROBLEMSETMANAGER_H
