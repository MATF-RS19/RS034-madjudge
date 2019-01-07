#include "cProblemSolution.h"

#ifdef SERVER
CProblemSolution::CProblemSolution() : ClientPacket(EClientPacketType::ProblemSolution){}

void CProblemSolution::Deserialize()
{
    _problemID = _ds.Read<qint32>();
    _text = _ds.ReadString();
}

void CProblemSolution::Process()
{
    //send to evaluation
}
#endif

#ifdef CLIENT
CProblemSolution::CProblemSolution(qint32 problemID, const QString& text) : ClientPacket(EClientPacketType::ProblemSolution), _problemID(problemID), _text(text){}

QByteArray CProblemSolution::Serialize()
{
    _ds.Write(_problemID);
    _ds.WriteString(_text);
    return ClientPacket::Serialize();
}
#endif
