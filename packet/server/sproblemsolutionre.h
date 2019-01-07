#ifndef SPROBLEMSOLUTIONRE_H
#define SPROBLEMSOLUTIONRE_H

#include "serverpacket.h"

class SProblemSolutionRe : public ServerPacket
{
public:

#ifdef CLIENT
    SProblemSolutionRe();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SProblemSolutionRe(qint64 timestamp);       //TO-DO
    QByteArray Serialize();
#endif

private:
    qint64 _timestamp;
};

#endif // SPROBLEMSOLUTIONRE_H
