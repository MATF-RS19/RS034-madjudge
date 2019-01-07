#include "skeepalive.h"

#ifdef CLIENT
#include "user.h"

SKeepAlive::SKeepAlive() : ServerPacket(EServerPacketType::KeepAlive){}

void SKeepAlive::Deserialize()
{
    m_timestamp = m_ds.Read<time_t>();
}

void SKeepAlive::Process()
{
    User* user = User::Get();
    user->KeepAlive(m_timestamp);
}
#endif

#ifdef SERVER
SKeepAlive::SKeepAlive(time_t timestamp) : ServerPacket(EServerPacketType::KeepAlive), m_timestamp(timestamp){}

QByteArray SKeepAlive::Serialize()
{
    m_ds.Write(m_timestamp);
    return ServerPacket::Serialize();
}
#endif
