#include "scontestapplyre.h"

#ifdef CLIENT
#include "user.h"

SContestApplyRe::SContestApplyRe() : ServerPacket(EServerPacketType::ContestApplyResult){}

void SContestApplyRe::Deserialize()
{
    m_result = m_ds.Read<bool>();
    if (m_result != 0)
    {
        m_error = m_ds.ReadString();
    }
}

void SContestApplyRe::Process()
{
    User* user = User::Get();
    if (m_result)
    {
        user->ConfirmContestApply();
    }
    else
    {
        user->ReportError(m_error);
    }
}
#endif

#ifdef SERVER
SContestApplyRe::SContestApplyRe(bool result, const QString& error) : ServerPacket(EServerPacketType::ContestApplyResult), m_result(result), m_error(error){}

QByteArray SContestApplyRe::Serialize()
{
    m_ds.Write(m_result);
    if (m_result != 0)
    {
        m_ds.WriteString(m_error);
    }
    return ServerPacket::Serialize();
}
#endif
