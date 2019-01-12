#ifndef SUSERLEAVECONTEST_H
#define SUSERLEAVECONTEST_H

#include "serverpacket.h"

class SUserLeaveContest : public ServerPacket
{
public:

#ifdef CLIENT
    SUserLeaveContest();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SUserLeaveContest(qint32 userID);
    QByteArray Serialize();
#endif

private:
    qint32 m_userID;
};

#endif // SUSERLEAVECONTEST_H
