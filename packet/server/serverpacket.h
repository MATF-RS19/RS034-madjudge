#ifndef SERVERPACKET_H
#define SERVERPACKET_H

#include "net\packet\packet.h"

class ServerPacket : public Packet
{
public:
    enum EServerPacketType
    {
        //Before a user is logged in
        KeepAlive = 0,                      //response to the client keepalive, verifies that the connection is still alive
        LoginResult,                        //sends login result = success or failed (if the name is already taken)

        //Not in contest/exercise mode but logged in
        ContestApplyResult,                 //sends result of the contest/exercise application = success or failed (timed out etc.)
        ContestInfo,                        //in case the contest is a regular one, sends info about other player names/IDs that will participate in the contest

        //Contest/Exercise mode
        ProblemSet,                         //sends all the problems as text, ideally it should happen 1 or 2 minutes after all the players are in the contest, so they would have time to prepare
        ProblemSolutionResult,              //sends result of the solution that the user sent = success or failed (TLE, MLE, WA etc.)
        UpdateUserScore,                    //sends update of user score to all the users in the current contest - it can be sent periodically or whenever a user's score changes, we'll see
        LeaveResult,                        //sends result of leave request
        UserLeaveContest,                   //sends information that a user has left the contest to other participants

        //Add more later maybe..
    };

    ServerPacket(EServerPacketType type) : _type(type){}

    qint32 GetType();

#ifdef CLIENT
    static ServerPacket* Decode(DataStream& ds);
#endif

private:
    EServerPacketType _type;
};

#endif // SERVERPACKET_H
