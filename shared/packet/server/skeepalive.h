#ifndef SKEEPALIVE_H
#define SKEEPALIVE_H

#include "serverpacket.h"

class SKeepAlive : public ServerPacket
{
public:

#ifdef CLIENT
    SKeepAlive();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SKeepAlive(time_t timestamp);
    QByteArray Serialize();
#endif

private:
    time_t m_timestamp;
};

#endif // SKEEPALIVE_H
