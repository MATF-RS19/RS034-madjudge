#include "ckeepalive.h"

#ifdef SERVER
#include "contestant.h"

CKeepAlive::CKeepAlive() : ClientPacket(EClientPacketType::KeepAlive){}

void CKeepAlive::Deserialize()
{
    m_timestamp = m_ds.Read<time_t>();
}

void CKeepAlive::Process()
{
    m_user->KeepAlive(m_timestamp);
}
#endif

#ifdef CLIENT
CKeepAlive::CKeepAlive(time_t timestamp) : ClientPacket(EClientPacketType::KeepAlive), m_timestamp(timestamp){}

QByteArray CKeepAlive::Serialize()
{
    m_ds.Write(m_timestamp);
    return ClientPacket::Serialize();
}
#endif
