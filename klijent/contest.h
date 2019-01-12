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

    //getteri i setteri
    qint32 GetWaitingTime() const;
    qint32 GetContestDuration() const;
    qint32 GetNumberOfProblems() const;
    QMap<qint32, QString> GetContestants() const;
    QMap<qint32, QString> GetContestantScores() const;

    QList<ProblemEntry*> GetProblemset() const;
    void SetProblemset(const QList<ProblemEntry*>& problemset);

    void RemoveParticipant(qint32 ID);                              //sklanja korisnika iz takmičenja
    void UpdateParticipantScore(qint32 ID, qint32 score);           //ažurira rezultat takmičara

private:
    qint32 m_waitingTime;                                           //vreme čekanja da se pripreme svi igrači
    qint32 m_contestDuration;                                       //trajanje takmičenja
    qint32 m_numberOfProblems;                                      //broj problema

    QMap<qint32, ParticipantInfo> m_contestants;                    //takmičari koji se trenutno takmičenje
    QMap<qint32, ProblemEntry*> m_problemset;                       //problemi
};

#endif // CONTEST_H
