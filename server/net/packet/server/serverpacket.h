#ifndef SERVERPACKET_H
#define SERVERPACKET_H

#include "packet.h"

class ServerPacket : Packet
{
public:
    enum EServerPacketType
    {
        //TO-DO
        KeepAlive = 0,
    };

    ServerPacket(EServerPacketType type, Contestant* user, DataStream* ds) : Packet(user, ds), _type(type){}
    virtual ~ServerPacket();

    virtual void Serialize() = 0;

private:
    EServerPacketType _type;
};

#endif // SERVERPACKET_H
