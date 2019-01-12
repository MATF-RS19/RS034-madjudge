#include "scontestfinished.h"

#ifdef CLIENT
#include "user.h"
#include "contest.h"

SContestFinished::SContestFinished() : ServerPacket(EServerPacketType::ContestFinished){}

void SContestFinished::Deserialize()
{
}

void SContestFinished::Process()
{
    User* user = User::Get();
    if (user->IsInContest())
    {
        user->EndContest();
    }
}
#endif

#ifdef SERVER
SContestFinished::SContestFinished() : ServerPacket(EServerPacketType::ContestFinished) {}

QByteArray SContestFinished::Serialize()
{
    return ServerPacket::Serialize();
}
#endif
