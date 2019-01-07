#include "serverpacket.h"

#ifdef CLIENT
ServerPacket* ServerPacket::Decode(DataStream& ds)
{
    ServerPacket* returnPkt;
    EServerPacketType type = ds.Read<EServerPacketType>();
    DataStream packetDs(ds.ReadPackedBytes());
    if (packetDs.GetError() != DataStream::Ok)
    {
        return nullptr;
    }
    switch (type)
    {
        //TO-DO
    }
    returnPkt->SetDs(packetDs);
    return returnPkt;
}
#endif

qint32 ServerPacket::GetType()
{
    return _type;
}
