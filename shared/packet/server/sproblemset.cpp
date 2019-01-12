#include "sproblemset.h"

#ifdef CLIENT
#include "user.h"

SProblemSet::SProblemSet() : ServerPacket(EServerPacketType::ProblemSet){}

void SProblemSet::Deserialize()
{
    qint32 problemCount = m_ds.Read<qint32>();
    for (int i = 0; i < problemCount; i++)
    {
        qint32 ID = m_ds.Read<qint32>();
        qint32 timeLimit = m_ds.Read<qint32>();
        qint32 memoryLimit = m_ds.Read<qint32>();
        QString description = m_ds.ReadString();
        m_problemset.push_back(new ProblemEntry(ID, timeLimit, memoryLimit, description));
    }
}

void SProblemSet::Process()
{
    User* user = User::Get();
    user->StartContest(m_problemset);
}
#endif

#ifdef SERVER
SProblemSet::SProblemSet(const QList<ProblemEntry*> &problemset)
    : ServerPacket(EServerPacketType::ProblemSet), m_problemset(problemset){}

QByteArray SProblemSet::Serialize()
{
    m_ds.Write<qint32>(m_problemset.size());
    for (const auto& problem : m_problemset)
    {
        m_ds.Write<qint32>(problem->GetID());
        m_ds.Write<qint32>(problem->GetTimeLimit());
        m_ds.Write<qint32>(problem->GetMemoryLimit());
        m_ds.WriteString(problem->GetDescription());
    }
    return ServerPacket::Serialize();
}
#endif
