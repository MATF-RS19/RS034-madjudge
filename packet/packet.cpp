#include "packet.h"

void Packet::SetDs(const DataStream& ds)
{
    _ds = ds;
}

#ifdef SERVER
void Packet::SetUser(Contestant* user)
{
    _user = user;
}
#endif

QByteArray Packet::Serialize()
{
    DataStream finalDs;
    QByteArray internalData = _ds.ReadAll();
    finalDs.Write(internalData.size() + 4);
    finalDs.Write(GetType());
    finalDs.WriteBytes(internalData);
    return finalDs.ReadAll();
}
