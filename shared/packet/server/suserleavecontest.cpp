#include "suserleavecontest.h"

#ifdef CLIENT
#include "user.h"
#include "contest.h"

SUserLeaveContest::SUserLeaveContest() : ServerPacket(EServerPacketType::UserLeaveContest){}

void SUserLeaveContest::Deserialize()
{
    m_userID = m_ds.Read<qint32>();
}

void SUserLeaveContest::Process()
{
    User* user = User::Get();
    if (user->IsInContest())
    {
        Contest* contest = user->GetCurrentContest();
        contest->RemoveParticipant(m_userID);
        emit user->UpdatedContestants();
    }
}
#endif

#ifdef SERVER
SUserLeaveContest::SUserLeaveContest(qint32 userID) : ServerPacket(EServerPacketType::UserLeaveContest), m_userID(userID){}

QByteArray SUserLeaveContest::Serialize()
{
    m_ds.Write<qint32>(m_userID);
    return ServerPacket::Serialize();
}
#endif
