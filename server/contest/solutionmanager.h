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

    void CheckSolution(const QString& text, Contestant* contestant, ProblemEntry* entry);           //pokreće novu nit koja će proveriti rešenje koje se nalazi u text, za takmičara contestant i problem entry

signals:
    void SolutionStatusUpdated(Contestant* contestant, ProblemEntry::ESolutionStatus status);       //emituje se svaki put kada se promeni status provere rešenja

private:
    void InternalCheckSolution(const QString& text, Contestant* contestant, ProblemEntry* entry);   //proverava rešenje zadatka
};

#endif // SOLUTIONMANAGER_H
