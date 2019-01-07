#ifndef CLOGIN_H
#define CLOGIN_H

#include "net/packet/client/clientpacket.h"

class CLogin : public ClientPacket
{
#ifdef SERVER
    CLogin();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CLogin(const QString& name);
    QByteArray Serialize();
#endif

private:
    QString _name;
};

#endif // CLOGIN_H
