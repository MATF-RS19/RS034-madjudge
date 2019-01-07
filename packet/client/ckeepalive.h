#ifndef CKEEPALIVE_H
#define CKEEPALIVE_H

#include "net/packet/client/clientpacket.h"

class CKeepAlive : public ClientPacket
{
public:

#ifdef SERVER
    CKeepAlive();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CKeepAlive(qint64 timestamp);
    QByteArray Serialize();
#endif

private:
    qint64 _timestamp;
};

#endif // CKEEPALIVE_H
