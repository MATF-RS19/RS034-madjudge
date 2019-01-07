#include "sloginre.h"

#ifdef CLIENT
SLoginRe::SLoginRe() : ServerPacket(EServerPacketType::LoginResult){}

void SLoginRe::Deserialize()
{
    _result = _ds.Read<bool>();
    if (_result != 0)
    {
        _error = _ds.ReadString();
    }
}

void SLoginRe::Process()
{
    //open main menu or whatever if result is ok, otherwise show string error
}
#endif

#ifdef SERVER
SLoginRe::SLoginRe(bool result, const QString& error) : ServerPacket(EServerPacketType::LoginResult), _result(result), _error(error){}

QByteArray SLoginRe::Serialize()
{
    _ds.Write(_result);
    if (_result != 0)
    {
        _ds.WriteString(_error);
    }
    return ServerPacket::Serialize();
}
#endif
