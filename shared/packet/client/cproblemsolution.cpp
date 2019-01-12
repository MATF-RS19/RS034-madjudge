#include "cproblemsolution.h"

#ifdef SERVER
#include "contestant.h"

CProblemSolution::CProblemSolution() : ClientPacket(EClientPacketType::ProblemSolution){}

void CProblemSolution::Deserialize()
{
    m_problemID = m_ds.Read<qint32>();
    m_text = m_ds.ReadString();
}

void CProblemSolution::Process()
{
    m_user->VerifySolution(m_problemID, m_text);
}
#endif

#ifdef CLIENT
CProblemSolution::CProblemSolution(qint32 problemID, const QString& text) : ClientPacket(EClientPacketType::ProblemSolution), m_problemID(problemID), m_text(text){}

QByteArray CProblemSolution::Serialize()
{
    m_ds.Write(m_problemID);
    m_ds.WriteString(m_text);
    return ClientPacket::Serialize();
}
#endif
