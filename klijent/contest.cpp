#include "contest.h"
#include "../shared/problementry.h"
#include <QDebug>

Contest::Contest(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QMap<qint32, QString> &contestants)
    : m_waitingTime(waitingTime), m_contestDuration(contestDuration), m_numberOfProblems(numberOfProblems)
{
    qDebug() << "Make contest, participants:";
    ParticipantInfo participant;
    participant.m_score = 0;
    for (auto contestantKVP = contestants.cbegin(); contestantKVP != contestants.cend(); contestantKVP++)
    {
        participant.m_name = contestantKVP.value();
        qDebug() << contestantKVP.key() << participant.m_name;
        m_contestants.insert(contestantKVP.key(), participant);
    }
}

qint32 Contest::GetWaitingTime() const
{
    return m_waitingTime;
}

qint32 Contest::GetContestDuration() const
{
    return m_contestDuration;
}

qint32 Contest::GetNumberOfProblems() const
{
    return m_numberOfProblems;
}

QList<ProblemEntry *> Contest::GetProblemset() const
{
    return m_problemset.values();
}

void Contest::SetProblemset(const QList<ProblemEntry *> &problemset)
{
    for (const auto& problem : problemset)
    {
        m_problemset.insert(problem->GetID(), problem);
    }
}

void Contest::RemoveParticipant(qint32 ID)
{
    m_contestants.remove(ID);
}

void Contest::UpdateParticipantScore(qint32 ID, qint32 score)
{
    m_contestants[ID].m_score += score;
}
