#include "clogin.h"

#ifdef SERVER
CLogin::CLogin() : ClientPacket(EClientPacketType::Login){}

void CLogin::Deserialize()
{
    _name = _ds.ReadString();
}

void CLogin::Process()
{
    //_user->Login()
}
#endif

#ifdef CLIENT
CLogin::CLogin(const QString& name) : ClientPacket(EClientPacketType::Login), _name(name){}

QByteArray CLogin::Serialize()
{
    _ds.WriteString(_name);
    return ClientPacket::Serialize();
}
#endif
