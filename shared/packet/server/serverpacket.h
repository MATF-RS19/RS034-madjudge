#ifndef SERVERPACKET_H
#define SERVERPACKET_H

#include "../packet.h"

class ServerPacket : public Packet
{
public:
    enum EServerPacketType
    {
        //Na početku
        KeepAlive = 0,                      //odgovor na klijentski keepalive, pomaže sa održavanjem konekcije aktivnom
        LoginResult,                        //vraća rezultat pokušaja prijavljivanja

        //Nema takmičenja ali korisnik je prijavljen
        ContestApplyResult,                 //odgovor pokušaja prijave za takmičenje

        //Tokom takmičenja
        ContestInfo,                        //informacije o takmičenje, takođe javlja korisniku da je uparen sa drugim takmičarima i da će takmičenje da uskoro počne
        ProblemSet,                         //šalje sve probleme u tekstualnoj formi i javlja takmičaru da takmičenje sada počinje

        ProblemSolutionStatus,              //vraća status rešenja koje se trenutno proverava
        UpdateUserScore,                    //šalje rezultat (score) nekog korisnika kad god se on promeni
        LeaveResult,                        //vraća rezultat pokušaja napuštanja takmičenja
        UserLeaveContest,                   //šalje informaciju da je takmičar napustio takmičenje drugim takmičarima

        ContestFinished                     //šalje informaciju da je takmičenje završeno svim takmičarima
    };

    ServerPacket(EServerPacketType type) : m_type(type){}

    qint32 GetType();

#ifdef CLIENT
    static ServerPacket* Decode(DataStream& ds, PacketError& error);
#endif

private:
    EServerPacketType m_type;
};

#endif // SERVERPACKET_H
