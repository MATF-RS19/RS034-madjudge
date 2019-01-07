 #include "packet.h"

void Packet::SetDs(const DataStream& ds)
{
    m_ds = ds;
}

#ifdef SERVER
void Packet::SetUser(Contestant* user)
{
    m_user = user;
}
#endif

QByteArray Packet::Serialize()
{
    DataStream finalDs;
    QByteArray internalData = m_ds.ReadAll();
    finalDs.Write(GetType());
    finalDs.WritePackedBytes(internalData);
    return finalDs.ReadAll();
}

void Packet::Send(Client *client, Packet* packet)
{
    QByteArray packetData = packet->Serialize();
    client->Send(packetData);
}
