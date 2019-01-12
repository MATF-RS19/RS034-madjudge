#include "scontestinfo.h"
#ifdef SERVER
#include "contestant.h"
#endif

#ifdef CLIENT
#include "user.h"

SContestInfo::SContestInfo() : ServerPacket(EServerPacketType::ContestInfo){}

void SContestInfo::Deserialize()
{
    m_waitingTime = m_ds.Read<qint32>();
    m_contestDuration = m_ds.Read<qint32>();
    m_numberOfProblems = m_ds.Read<qint32>();
    qint32 contestantCount = m_ds.Read<qint32>();
    for (qint32 i = 0; i < contestantCount; i++)
    {
        qint32 contestantId = m_ds.Read<qint32>();
        QString contestantName = m_ds.ReadString();
        m_contestants.insert(contestantId, contestantName);
    }
}

void SContestInfo::Process()
{
    User* user = User::Get();
    user->MakeContest(m_waitingTime, m_contestDuration, m_numberOfProblems, m_contestants);
}
#endif

#ifdef SERVER
SContestInfo::SContestInfo(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QList<Contestant *> &contestants)
    : ServerPacket(EServerPacketType::ContestInfo),
      m_waitingTime(waitingTime), m_contestDuration(contestDuration), m_numberOfProblems(numberOfProblems), m_contestants(contestants){}

QByteArray SContestInfo::Serialize()
{
    m_ds.Write<qint32>(m_waitingTime);
    m_ds.Write<qint32>(m_contestDuration);
    m_ds.Write<qint32>(m_numberOfProblems);
    m_ds.Write<qint32>(m_contestants.size());
    for (const auto& contestant : m_contestants)
    {
        m_ds.Write<qint32>(contestant->GetID());
        m_ds.WriteString(contestant->GetName());
    }
    return ServerPacket::Serialize();
}
#endif
