#ifndef CONTEST_H
#define CONTEST_H

#include <QMap>

class ProblemEntry;

struct ParticipantInfo
{
    QString m_name;
    qint32 m_score;
};

class Contest
{
    enum EContestPhase                                          //faze takmičenja
    {
        Idle,                                                   //još uvek nije počelo
        Waiting,                                                //čeka se da se svi igrači pripreme
        InProgress,                                             //trenutno se dešava
        Finished                                                //završeno
    };

public:
    Contest(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QMap<qint32, QString>& contestants);

    qint32 GetWaitingTime() const;
    qint32 GetContestDuration() const;
    qint32 GetNumberOfProblems() const;

    QList<ProblemEntry*> GetProblemset() const;
    void SetProblemset(const QList<ProblemEntry*>& problemset);

    void RemoveParticipant(qint32 ID);
    void UpdateParticipantScore(qint32 ID, qint32 score);

private:
    qint32 m_waitingTime;
    qint32 m_contestDuration;
    qint32 m_numberOfProblems;

    QMap<qint32, ParticipantInfo> m_contestants;
    QMap<qint32, ProblemEntry*> m_problemset;                    //[ID, ProblemEntry]
};

#endif // CONTEST_H
