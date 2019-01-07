#include "skeepalive.h"

#ifdef CLIENT
SKeepAlive::SKeepAlive() : ServerPacket(EServerPacketType::KeepAlive){}

void SKeepAlive::Deserialize()
{
    _timestamp = _ds.Read<qint64>();
}

void SKeepAlive::Process()
{
    //get singleton of user
    //apply ping check or whatever and restart passive timer
}
#endif

#ifdef SERVER
SKeepAlive::SKeepAlive(qint64 timestamp) : ServerPacket(EServerPacketType::KeepAlive), _timestamp(timestamp){}

QByteArray SKeepAlive::Serialize()
{
    _ds.Write(_timestamp);
    return ServerPacket::Serialize();
}
#endif
