#include "scontestapplyre.h"

#ifdef CLIENT
SContestApplyRe::SContestApplyRe() : ServerPacket(EServerPacketType::ContestApplyResult){}

void SContestApplyRe::Deserialize()
{
    _result = _ds.Read<bool>();
    if (_result != 0)
    {
        _error = _ds.ReadString();
    }
}

void SContestApplyRe::Process()
{
    //open contest menu, otherwise show string error
}
#endif

#ifdef SERVER
SContestApplyRe::SContestApplyRe(bool result, const QString& error) : ServerPacket(EServerPacketType::ContestApplyResult), _result(result), _error(error){}

QByteArray SContestApplyRe::Serialize()
{
    _ds.Write(_result);
    if (_result != 0)
    {
        _ds.WriteString(_error);
    }
    return ServerPacket::Serialize();
}
#endif
