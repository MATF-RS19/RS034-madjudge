#ifndef CLEAVECONTEST_H
#define CLEAVECONTEST_H

#include "clientpacket.h"

class CLeaveContest : public ClientPacket
{
public:

#ifdef SERVER
    CLeaveContest();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CLeaveContest();
    QByteArray Serialize();
#endif
};

#endif // CLEAVECONTEST_H
