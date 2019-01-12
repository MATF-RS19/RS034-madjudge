#ifndef CLOGIN_H
#define CLOGIN_H

#include "clientpacket.h"

class CLogin : public ClientPacket
{
public:

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
    QString m_name;
};

#endif // CLOGIN_H
