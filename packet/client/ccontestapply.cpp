#include "ccontestapply.h"

#ifdef SERVER
CContestApply::CContestApply() : ClientPacket(EClientPacketType::ContestApply){}

void CContestApply::Deserialize()
{
    _type = _ds.Read<EContestApplyType>();
}

void CContestApply::Process()
{
    //get singleton of contestmanager, call to put user in queue if contest mode, if solo mode then just call solo exercise on user
    //if type is regular, then put player in queue and wait until there's enough players
    //if type is solo then just start it
}
#endif

#ifdef CLIENT
CContestApply::CContestApply(EContestApplyType type) : ClientPacket(EClientPacketType::ContestApply), _type(type){}

QByteArray CContestApply::Serialize()
{
    _ds.Write(_type);
    return ClientPacket::Serialize();
}
#endif
