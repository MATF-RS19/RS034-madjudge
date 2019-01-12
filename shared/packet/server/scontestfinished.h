#ifndef SCONTESTFINISHED_H
#define SCONTESTFINISHED_H

#include "serverpacket.h"

class SContestFinished : public ServerPacket
{
public:

#ifdef CLIENT
    SContestFinished();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SContestFinished();
    QByteArray Serialize();
#endif

};

#endif // SCONTESTFINISHED_H
