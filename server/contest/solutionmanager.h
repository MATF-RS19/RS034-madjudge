#ifndef SOLUTIONMANAGER_H
#define SOLUTIONMANAGER_H

#include <QObject>
#include "../shared/problementry.h"

class ProblemEntry;
class Contestant;

class SolutionManager : public QObject
{
    Q_OBJECT

    static SolutionManager* h_Inst;

public:
    static void Init();
    static void Shutdown();
    static SolutionManager* Get();

    void CheckSolution(const QString& text, Contestant* contestant, ProblemEntry* entry);

signals:
    void SolutionStatusUpdated(Contestant* contestant, ProblemEntry::ESolutionStatus status);

private:
    void InternalCheckSolution(const QString& text, Contestant* contestant, ProblemEntry* entry);
};

#endif // SOLUTIONMANAGER_H
