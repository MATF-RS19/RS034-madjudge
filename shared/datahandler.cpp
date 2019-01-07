#include "datahandler.h"
#include <QtConcurrent/QtConcurrent>

#ifdef SERVER
#include "contestant.h"
#include "../shared/packet/client/clientpacket.h"
#endif

#ifdef CLIENT
#include "../shared/packet/server/serverpacket.h"
#endif

#ifdef SERVER
DataHandler::DataHandler(Contestant *contestant) : m_contestant(contestant), m_processing(false){}
#endif

#ifdef CLIENT
DataHandler::DataHandler() : m_processing(false){}
#endif

void DataHandler::HandleData(const QByteArray &data)
{
    m_dataLock.lock();
    m_ds.WriteBytes(data);
    bool broken = false;
    Packet::PacketError decodeError;
    while (!broken && m_ds.CanRead())
    {
        Packet* packet = DecodePacket(m_ds, decodeError);
        if (packet == nullptr)
        {
            switch (decodeError)
            {
            case Packet::UnknownType:
            case Packet::DataMismatch:
                emit Error();
                return;
            case Packet::SizeMismatch:
                broken = true;
                break;
            }
        }
        else
        {
            packet->Deserialize();
            packet->Process();
            delete packet;
        }
    }
    if (!broken)
    {
        m_ds.Clear();
    }
    m_dataLock.unlock();
}

#ifdef SERVER
Packet* DataHandler::DecodePacket(DataStream &ds, Packet::PacketError& error)
{
    return ClientPacket::Decode(m_contestant, ds, error);
}
#endif

#ifdef CLIENT
Packet* DataHandler::DecodePacket(DataStream &ds, Packet::PacketError& error)
{
    return ServerPacket::Decode(ds, error);
}
#endif
