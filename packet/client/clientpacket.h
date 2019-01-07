#ifndef CLIENTPACKET_H
#define CLIENTPACKET_H

#include "net\packet\packet.h"

class ClientPacket : public Packet
{
public:
    enum EClientPacketType
    {
        //At the beginning
        KeepAlive = 0,                          //automatic check if the connection is still alive
        Login,                                  //"login" with name only, no password etc.

        //Needs to be logged in but not in the contest at the moment
        ContestApply,                           //apply to join contest or exercise (solo contest)

        //Needs to be in the contest or exercise mode at the moment
        ProblemSolution,                        //sends file/text to verify if the solution is correct
        LeaveContest,                           //leave contest/exercise and go back

        //Add more later maybe..
    };

    ClientPacket(EClientPacketType type) : _type(type){}

    qint32 GetType();

#ifdef SERVER
    static ClientPacket* Decode(Contestant* user, DataStream& ds);
#endif

private:
    EClientPacketType _type;
};

#endif // CLIENTPACKET_H
