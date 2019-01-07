#ifndef SLOGINRESULT_H
#define SLOGINRESULT_H

#include "serverpacket.h"

class SLoginRe : public ServerPacket
{
public:

#ifdef CLIENT
    SLoginRe();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SLoginRe(bool result, const QString& error = "");
    QByteArray Serialize();
#endif

private:
    bool m_result;
    QString m_error;
};

#endif // SLOGINRESULT_H
