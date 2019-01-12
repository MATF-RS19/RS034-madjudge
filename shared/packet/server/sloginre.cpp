#include "sloginre.h"

#ifdef CLIENT
#include "user.h"

SLoginRe::SLoginRe() : ServerPacket(EServerPacketType::LoginResult){}

void SLoginRe::Deserialize()
{
    m_result = m_ds.Read<bool>();
    if (m_result != 0)
    {
        m_error = m_ds.ReadString();
    }
}

void SLoginRe::Process()
{
    User* user = User::Get();
    if (m_result)
    {
        user->ConfirmLogin();
    }
    else
    {
        user->ReportError(m_error);
    }
}
#endif

#ifdef SERVER
SLoginRe::SLoginRe(bool result, const QString& error) : ServerPacket(EServerPacketType::LoginResult), m_result(result), m_error(error){}

QByteArray SLoginRe::Serialize()
{
    m_ds.Write(m_result);
    if (m_result != 0)
    {
        m_ds.WriteString(m_error);
    }
    return ServerPacket::Serialize();
}
#endif
