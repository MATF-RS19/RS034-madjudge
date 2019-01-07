#ifndef PACKET_H
#define PACKET_H

#include "datastream.h"
#include "contestant.h"

class Packet
{
public:
    Packet(Contestant* user, DataStream* ds) : _ds(ds), _user(user){}
    virtual ~Packet();

    virtual void Process() = 0;
private:
    DataStream* _ds;
    Contestant* _user;
};

#endif // PACKET_H
