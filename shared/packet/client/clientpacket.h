#ifndef CLIENTPACKET_H
#define CLIENTPACKET_H

#include "../packet.h"

class ClientPacket : public Packet
{
public:
    enum EClientPacketType
    {
        //Na početku
        KeepAlive = 0,                          //proverava i održava konekciju živom
        Login,                                  //prijavljuje se samo sa imenom

        //Mora da bude prijavljen ali ne u takmičenju
        ContestApply,                           //prijavljuje se za takmičenje

        //Mora da bude tokom takmičenja
        ProblemSolution,                        //proverava rešenje problema
        LeaveContest,                           //izlazi iz takmičenja
    };

    ClientPacket(EClientPacketType type) : m_type(type){}

    qint32 GetType();                           //override za gettype jer mora da vrati EClientPacketType

#ifdef SERVER
    static ClientPacket* Decode(Contestant* user, DataStream& ds, PacketError& error);              //pretvara podatke koji se nalaze u datastream u paket
#endif

private:
    EClientPacketType m_type;
};

#endif // CLIENTPACKET_H
