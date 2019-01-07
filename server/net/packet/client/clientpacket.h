#ifndef CLIENTPACKET_H
#define CLIENTPACKET_H

#include "packet.h"

class ClientPacket : Packet
{
public:
    enum EClientPacketType
    {
        //TO-DO
        KeepAlive = 0,
    };

    ClientPacket(EClientPacketType type, Contestant* user, DataStream* ds) : Packet(user, ds), _type(type){}
    virtual ~ClientPacket();

    virtual void Deserialize() = 0;

private:
    EClientPacketType _type;
};

#endif // CLIENTPACKET_H
