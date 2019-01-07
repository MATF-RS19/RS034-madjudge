#include "sleavere.h"

#ifdef CLIENT
SLeaveRe::SLeaveRe() : ServerPacket(EServerPacketType::LeaveResult){}

void SLeaveRe::Deserialize()
{
    _result = _ds.Read<bool>();
    if (_result != 0)
    {
        _error = _ds.ReadString();
    }
}

void SLeaveRe::Process()
{
    //open main menu or whatever if result is ok, otherwise show string error
}
#endif

#ifdef SERVER
SLeaveRe::SLeaveRe(bool result, const QString& error) : ServerPacket(EServerPacketType::LeaveResult), _result(result), _error(error){}

QByteArray SLeaveRe::Serialize()
{
    _ds.Write(_result);
    if (_result != 0)
    {
        _ds.WriteString(_error);
    }
    return ServerPacket::Serialize();
}
#endif
