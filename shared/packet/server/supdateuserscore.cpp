#include "supdateuserscore.h"

#ifdef CLIENT
#include "user.h"
#include "contest.h"

SUpdateUserScore::SUpdateUserScore() : ServerPacket(EServerPacketType::UpdateUserScore){}

void SUpdateUserScore::Deserialize()
{
    m_userID = m_ds.Read<qint32>();
    m_userScore = m_ds.Read<qint32>();
}

void SUpdateUserScore::Process()
{
    User* user = User::Get();
    if (user->IsInContest())
    {
        Contest* contest = user->GetCurrentContest();
        contest->UpdateParticipantScore(m_userID, m_userScore);
        emit user->UpdatedContestants();
    }
}
#endif

#ifdef SERVER
SUpdateUserScore::SUpdateUserScore(qint32 userID, qint32 userScore) : ServerPacket(EServerPacketType::UpdateUserScore), m_userID(userID), m_userScore(userScore){}

QByteArray SUpdateUserScore::Serialize()
{
    m_ds.Write<qint32>(m_userID);
    m_ds.Write<qint32>(m_userScore);
    return ServerPacket::Serialize();
}
#endif
