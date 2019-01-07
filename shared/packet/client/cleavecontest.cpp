#include "cleavecontest.h"

#ifdef SERVER
#include "contestant.h"

CLeaveContest::CLeaveContest() : ClientPacket(EClientPacketType::LeaveContest){}

void CLeaveContest::Deserialize()
{
}

void CLeaveContest::Process()
{
    m_user->LeaveContest();
}
#endif

#ifdef CLIENT
CLeaveContest::CLeaveContest() : ClientPacket(EClientPacketType::LeaveContest){}

QByteArray CLeaveContest::Serialize()
{
    return ClientPacket::Serialize();
}
#endif
