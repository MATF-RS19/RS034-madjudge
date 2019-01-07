#include "ccontestapply.h"

#ifdef SERVER
#include "contestant.h"

CContestApply::CContestApply() : ClientPacket(EClientPacketType::ContestApply){}

void CContestApply::Deserialize()
{
    m_type = m_ds.Read<EContestApplyType>();
}

void CContestApply::Process()
{
    switch (m_type)
    {
    case EContestApplyType::Regular:
        m_user->ApplyForContest();
        break;
    case EContestApplyType::Solo:
        m_user->ApplyForExercise();
        break;
    }
}
#endif

#ifdef CLIENT
CContestApply::CContestApply(EContestApplyType type) : ClientPacket(EClientPacketType::ContestApply), m_type(type){}

QByteArray CContestApply::Serialize()
{
    m_ds.Write(m_type);
    return ClientPacket::Serialize();
}
#endif
