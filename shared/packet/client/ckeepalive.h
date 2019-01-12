#ifndef CKEEPALIVE_H
#define CKEEPALIVE_H

#include "clientpacket.h"

class CKeepAlive : public ClientPacket
{
public:

#ifdef SERVER
    CKeepAlive();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CKeepAlive(time_t timestamp);
    QByteArray Serialize();
#endif

private:
    time_t m_timestamp;
};

#endif // CKEEPALIVE_H
