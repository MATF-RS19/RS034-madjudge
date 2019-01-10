#ifndef SPROBLEMSET_H
#define SPROBLEMSET_H

#include "serverpacket.h"
#include "../../problementry.h"
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
    SProblemSet(const QList<ProblemEntry*>& problemset);
    QByteArray Serialize();
#endif

private:
    QList<ProblemEntry*> m_problemset;
};

#endif // SPROBLEMSET_H
