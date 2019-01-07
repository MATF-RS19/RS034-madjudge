#include "sproblemsolutionre.h"

#ifdef CLIENT
SProblemSolutionRe::SProblemSolutionRe() : ServerPacket(EServerPacketType::ProblemSolutionResult){}

void SProblemSolutionRe::Deserialize()
{
    _timestamp = _ds.Read<qint64>();
}

void SProblemSolutionRe::Process()
{
    //get singleton of user
    //apply ping check or whatever and restart passive timer
}
#endif

#ifdef SERVER
SProblemSolutionRe::SProblemSolutionRe(qint64 timestamp) : ServerPacket(EServerPacketType::ProblemSolutionResult), _timestamp(timestamp){}

QByteArray SProblemSolutionRe::Serialize()
{
    _ds.Write(_timestamp);
    return ServerPacket::Serialize();
}
#endif
