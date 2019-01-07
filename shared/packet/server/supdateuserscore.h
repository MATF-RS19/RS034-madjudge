#ifndef SUPDATEUSERSCORE_H
#define SUPDATEUSERSCORE_H

#include "serverpacket.h"

class SUpdateUserScore : public ServerPacket
{
public:

#ifdef CLIENT
    SUpdateUserScore();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SUpdateUserScore(qint32 userID, qint32 userScore);       //TO-DO
    QByteArray Serialize();
#endif

private:
    qint32 m_userID;
    qint32 m_userScore;
};

#endif // SUPDATEUSERSCORE_H
