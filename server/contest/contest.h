#ifndef CONTEST_H
#define CONTEST_H

class Contestant;

#include <QObject>
#include <QMap>
#include <QTimer>

class Packet;
class Contestant;
class ProblemEntry;

class Contest : public QObject
{
    Q_OBJECT

    enum EContestPhase                                          //faze takmičenja
    {
        Idle,                                                   //još uvek nije počelo
        Waiting,                                                //čeka se da se svi igrači pripreme
        InProgress,                                             //trenutno se dešava
        Finished                                                //završeno
    };

public:
    explicit Contest(QSet<Contestant*> contestants);
    ~Contest();

    void Start();                                               //počinje takmičenje

    void SendContestInfo() const;                               //šalje informacije o takmičenju (imena takmičara pre svega) svim takmičarima
    void GenerateProblemset();                                  //generiše problemset i šalje ga svim takmičarima

    void KickUser(Contestant* contestant);                      //izbacuje takmičara sa takmičenja i šalje informaciju o tome
    void UpdateScore(Contestant* contestant, qint32 problemId);     //dodeljuje poene takmičaru i šalje informacije o tome svim takmičarima
    void FinishContest();                                       //šalje informaciju da je takmičenje završeno

    ProblemEntry* GetProblem(qint32 ID) const;                  //uzima zadatak sa odgovarajućim ID

private slots:
    void OnPhaseFinished();                                     //završena trenutna faza

signals:
    void ContestFinished();                                     //signalizira se contestmanager da je takmičenje završeno

private:
    void SendToAll(Packet* packet) const;

    qint32 m_waitingTime;                                       //vreme koje čekamo da takmičenje počne od trenutka kad je napravljeno (ms)
    qint32 m_contestDuration;                                   //vreme koliko traje takmičenje (ms)
    qint32 m_numberOfProblems;                                  //ukupan broj problema

    QMap<Contestant*, qint32> m_contestants;                    //čuva takmičare i njihove rezultate
    EContestPhase m_currentPhase;                               //trenutna faza takmičenja
    QMap<qint32, ProblemEntry*> m_problemset;                   //čuva probleme sortirane po ID

    QTimer m_contestTimer;                                      //odbrojava vreme za čekanje i vreme koliko traje contest
};

#endif // CONTEST_H
