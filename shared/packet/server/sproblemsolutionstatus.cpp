#include "sproblemsolutionstatus.h"

#ifdef CLIENT
#include "user.h"

SProblemSolutionStatus::SProblemSolutionStatus() : ServerPacket(EServerPacketType::ProblemSolutionStatus){}

void SProblemSolutionStatus::Deserialize()
{
    m_status = m_ds.Read<ProblemEntry::ESolutionStatus>();
}

void SProblemSolutionStatus::Process()
{
    User* user = User::Get();
    QString statusMessage = "";
    switch (m_status)
    {
    case ProblemEntry::ESolutionStatus::InQueue:
        statusMessage = "In queue...";
        break;
    case ProblemEntry::ESolutionStatus::Compiling:
        statusMessage = "Compiling...";
        break;
    case ProblemEntry::ESolutionStatus::Running:
        statusMessage = "Running...";
        break;
    case ProblemEntry::ESolutionStatus::CompilationError:
        statusMessage = "Compilation error.";
        break;
    case ProblemEntry::ESolutionStatus::WrongAnswer:
        statusMessage = "Wrong answer.";
        break;
    case ProblemEntry::ESolutionStatus::TimeLimitExceeded:
        statusMessage = "Time limit exceeded.";
        break;
    case ProblemEntry::ESolutionStatus::MemoryLimitExceeded:
        statusMessage = "Memory limit exceeded.";
        break;
    case ProblemEntry::ESolutionStatus::Solved:
        statusMessage = "Correct!";
        break;
    }
    user->UpdateSolutionStatus(statusMessage);
}
#endif

#ifdef SERVER
SProblemSolutionStatus::SProblemSolutionStatus(ProblemEntry::ESolutionStatus status) : ServerPacket(EServerPacketType::ProblemSolutionStatus), m_status(status){}

QByteArray SProblemSolutionStatus::Serialize()
{
    m_ds.Write(m_status);
    return ServerPacket::Serialize();
}
#endif
