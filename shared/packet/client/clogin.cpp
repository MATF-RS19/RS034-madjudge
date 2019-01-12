#include "clogin.h"

#ifdef SERVER
#include "contestant.h"

CLogin::CLogin() : ClientPacket(EClientPacketType::Login){}

void CLogin::Deserialize()
{
    m_name = m_ds.ReadString();
}

void CLogin::Process()
{
    m_user->SetName(m_name);
}
#endif

#ifdef CLIENT
CLogin::CLogin(const QString& name) : ClientPacket(EClientPacketType::Login), m_name(name){}

QByteArray CLogin::Serialize()
{
    m_ds.WriteString(m_name);
    return ClientPacket::Serialize();
}
#endif
