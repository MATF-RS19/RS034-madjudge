#ifndef SCONTESTINFO_H
#define SCONTESTINFO_H

#include "serverpacket.h"

class SContestInfo : public ServerPacket
{
public:

#ifdef CLIENT
    SContestInfo();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SContestInfo(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QList<Contestant*>& contestants);
    QByteArray Serialize();
#endif

private:
    qint32 m_waitingTime;
    qint32 m_contestDuration;
    qint32 m_numberOfProblems;

#ifdef CLIENT
    QMap<qint32, QString> m_contestants;
#endif

#ifdef SERVER
    QList<Contestant*> m_contestants;
#endif

};

#endif // SContestInfo_H
