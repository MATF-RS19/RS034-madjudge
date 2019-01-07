#include "ckeepalive.h"

#ifdef SERVER
CKeepAlive::CKeepAlive() : ClientPacket(EClientPacketType::KeepAlive){}

void CKeepAlive::Deserialize()
{
    _timestamp = _ds.Read<qint64>();
}

void CKeepAlive::Process()
{
    //_user->ResetPassiveTimer();
    //_user->SendKeepAlive(_timestamp);
}
#endif

#ifdef CLIENT
CKeepAlive::CKeepAlive(qint64 timestamp) : ClientPacket(EClientPacketType::KeepAlive), _timestamp(timestamp){}

QByteArray CKeepAlive::Serialize()
{
    _ds.Write(_timestamp);
    return ClientPacket::Serialize();
}
#endif
