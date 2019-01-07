#ifndef SLEAVERE_H
#define SLEAVERE_H

#include "serverpacket.h"

class SLeaveRe : public ServerPacket
{
public:

#ifdef CLIENT
    SLeaveRe();
    void Deserialize();
    void Process();
#endif

#ifdef SERVER
    SLeaveRe(bool result, const QString& error = "");
    QByteArray Serialize();
#endif

private:
    bool _result;
    QString _error;
};

#endif // SLEAVERE_H
