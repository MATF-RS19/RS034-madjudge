#pragma once

#include "../client.h"

#ifdef SERVER
class Contestant;
#endif

class Packet
{
public:

    enum PacketError
    {
        UnknownType,
        DataMismatch,
        SizeMismatch
    };

    Packet(){}
    virtual ~Packet(){}

    void SetDs(const DataStream& ds);
#ifdef SERVER
    void SetUser(Contestant* user);
#endif

    virtual qint32 GetType() = 0;

    virtual void Deserialize(){}
    virtual void Process(){}
    virtual QByteArray Serialize();

    static void Send(Client* client, Packet* packet);

protected:
    DataStream m_ds;
#ifdef SERVER
    Contestant* m_user;
#endif
};
