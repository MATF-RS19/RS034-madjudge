#ifndef SPROBLEMSOLUTIONSTATUS_H
#define SPROBLEMSOLUTIONSTATUS_H

#include "serverpacket.h"
#include "../shared/problementry.h"

class SProblemSolutionStatus : public ServerPacket
{
public:

#ifdef CLIENT
    SProblemSolutionStatus();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SProblemSolutionStatus(ProblemEntry::ESolutionStatus status);
    QByteArray Serialize();
#endif

private:
    ProblemEntry::ESolutionStatus m_status;
};

#endif // SPROBLEMSOLUTIONSTATUS_H
