#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "datastream.h"
#include <QMutex>
#include <QQueue>
#include "packet/packet.h"

class Contestant;

class DataHandler : public QObject
{
    Q_OBJECT
public:

#ifdef SERVER
    DataHandler(Contestant* contestant);
#endif

#ifdef CLIENT
    DataHandler();
#endif

    void HandleData(const QByteArray& data);                                        //stavlja primljene podatke u queue i započinje procesovanje ako se ono trenutno ne odigrava

signals:
    void Error();                                                                   //desila se greška prilikom čitanja paketa

private:
    Packet* DecodePacket(DataStream& ds, Packet::PacketError& error);               //pretvara podatke u paket u zavisnosti od toga da li je definisano u serveru ili klijentu

#ifdef SERVER
    Contestant* m_contestant;
#endif

    DataStream m_ds;                                                                 //unutrašnji DataStream, koristi se kako bi se čuvali prethodni primljeni podaci u slučaju lomljenja paketa

    QQueue<QByteArray> m_toProcess;                                                  //queue podataka koji treba da se procesuiraju
    bool m_processing;                                                               //da li je procesuiranje u toku ili ne
    QMutex m_dataLock;                                                               //zaključava deo kod koji vrši procesuiranje kako ne bi došlo do pometnje jer više niti može da pristupi queue
};

#endif // DATAHANDLER_H
