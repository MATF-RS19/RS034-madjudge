#ifndef SCONTESTAPPLYRESULT_H
#define SCONTESTAPPLYRESULT_H

#include "serverpacket.h"

class SContestApplyRe : public ServerPacket
{
public:

#ifdef CLIENT
    SContestApplyRe();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SContestApplyRe(bool result, const QString& error = "");
    QByteArray Serialize();
#endif

private:
    bool m_result;
    QString m_error;
};

#endif // SCONTESTAPPLYRESULT_H
