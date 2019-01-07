#ifndef SKEEPALIVE_H
#define SKEEPALIVE_H

#include "net/packet/server/serverpacket.h"

class SKeepAlive : public ServerPacket
{
public:

#ifdef CLIENT
    SKeepAlive();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SKeepAlive(qint64 timestamp);
    QByteArray Serialize();
#endif

private:
    qint64 _timestamp;
};

#endif // SKEEPALIVE_H
