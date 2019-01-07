#include "sleavere.h"

#ifdef CLIENT
#include "user.h"

SLeaveRe::SLeaveRe() : ServerPacket(EServerPacketType::LeaveResult){}

void SLeaveRe::Deserialize()
{
    m_result = m_ds.Read<bool>();
    if (m_result != 0)
    {
        m_error = m_ds.ReadString();
    }
}

void SLeaveRe::Process()
{
    User* user = User::Get();
    if (m_result)
    {
        user->ConfirmLeaveContest();
    }
    else
    {
        user->ReportError(m_error);
    }
}
#endif

#ifdef SERVER
SLeaveRe::SLeaveRe(bool result, const QString& error) : ServerPacket(EServerPacketType::LeaveResult), m_result(result), m_error(error){}

QByteArray SLeaveRe::Serialize()
{
    m_ds.Write(m_result);
    if (m_result != 0)
    {
        m_ds.WriteString(m_error);
    }
    return ServerPacket::Serialize();
}
#endif
