#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QString>
#include <iostream>
#include <QQueue>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include "../shared/datastream.h"


class Client : public QObject
{
    Q_OBJECT
public:

#ifdef CLIENT
    explicit Client(const QString &host, quint16 port, QObject* parent = nullptr);
#endif

#ifdef SERVER
    explicit Client(QTcpSocket* socket, QObject* parent = nullptr);
#endif

    ~Client();

    void Connect();                                                                     //povezuje se sa serverom
    void Send(const QByteArray &data);                                                  //šalje podatke koji se nalaze u data
    void Disconnect();                                                                  //prekida konekciju sa serverom

private slots:
    void OnReadyRead();                                                                 //spreman za čitanje podataka koje je poslao server
    void OnError(QAbstractSocket::SocketError error);                                   //desila se greška u vezi sa serverom

signals:
    void Connected();                                                                   //povezao se sa serverom
    void DataReceived(QByteArray data);                                                 //primio podatke od servera
    void Disconnected();                                                                //veza se prekinula
    void Error();                                                                       //desila se greška u vezi

private:

    QHostAddress m_host;                                                                //IP adresa servera
    quint16 m_port;                                                                     //port servera
    QTcpSocket* m_socket;                                                               //soket koji se koristi za povezivanje sa serverom
};

#endif // CLIENT_H
