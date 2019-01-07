#include "clientpacket.h"

#ifdef SERVER
ClientPacket* ClientPacket::Decode(Contestant *user, DataStream &ds)
{
    ClientPacket* returnPkt;
    EClientPacketType type = ds.Read<EClientPacketType>();
    DataStream packetDs(ds.ReadPackedBytes());
    if (packetDs.GetError() != DataStream::Ok)
    {
        return nullptr;
    }
    switch (type)
    {
        //TO-DO
    }
    returnPkt->SetUser(user);
    returnPkt->SetDs(packetDs);
    return returnPkt;
}
#endif

qint32 ClientPacket::GetType()
{
    return _type;
}
