#pragma once

#include "../client.h"

#ifdef SERVER
class Contestant;
#endif

class Packet
{
public:

    enum PacketError
    {
        UnknownType,
        DataMismatch,
        SizeMismatch
    };

    Packet(){}
    virtual ~Packet(){}

    void SetDs(const DataStream& ds);
#ifdef SERVER
    void SetUser(Contestant* user);
#endif

    virtual qint32 GetType() = 0;                                   //tip paketa (može biti EServerPacketType ili EClientPacketType pa mora da se vrati u obliku qint32)

    virtual void Deserialize(){}                                    //pretvara podatke sa data streama koji se prosledi paketu u smislene informacije
    virtual void Process(){}                                        //procesuira ovaj paket, tj. tumači šta se radi njim
    virtual QByteArray Serialize();                                 //pretvara podatke koji se nalaze u paketu u bajtov

    static void Send(Client* client, Packet* packet);               //šalje packet client-u

protected:
    DataStream m_ds;                                                //unutrašnji data stream, ako paket treba da se čita onda se unapred postavlja, ako paket treba da se serijalajzuje onda se na njega piše u Serialize()
#ifdef SERVER
    Contestant* m_user;                                             //korisnik koji je primio ovaj paket
#endif
};
