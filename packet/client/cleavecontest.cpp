#include "cleavecontest.h"

#ifdef SERVER
CLeaveContest::CLeaveContest() : ClientPacket(EClientPacketType::LeaveContest){}

void CLeaveContest::Deserialize()
{
}

void CLeaveContest::Process()
{
    //_user->LeaveContest()
}
#endif

#ifdef CLIENT
//CLeaveContest::CLeaveContest() : ClientPacket(EClientPacketType::LeaveContest){}

QByteArray CLeaveContest::Serialize()
{
    return ClientPacket::Serialize();
}
#endif
