#ifndef PACKET_H
#define PACKET_H

#include "datastream.h"
#include "contestant.h"

class Packet
{
public:
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

protected:
    DataStream _ds;
#ifdef SERVER
    Contestant* _user;
#endif
};

#endif // PACKET_H
