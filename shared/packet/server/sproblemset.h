#ifndef SPROBLEMSET_H
#define SPROBLEMSET_H

#include "serverpacket.h"
#include <QMap>

class SProblemSet : public ServerPacket
{
public:

#ifdef CLIENT
    SProblemSet();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SProblemSet(QList<ProblemEntry*> problemset);
    QByteArray Serialize();
#endif

private:
    qint32 _waitingTime;
    qint32 _contestDuration;
    qint32 _numberOfProblems;

#ifdef CLIENT
    QMap<qint32, QString> _contestants;
#endif

#ifdef SERVER
    QList<ProblemEntry*> _problemset;
#endif
};

#endif // SPROBLEMSET_H
