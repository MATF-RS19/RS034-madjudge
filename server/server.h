#ifndef SERVER_H
#define SERVER_H

#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QMutex>
#include <QSet>
#include "contestant.h"
#include "../shared/client.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QString host, quint16 port);
    ~Server();

    void Start();                                               //pokreće server

private slots:
    void OnNewConnection();                                     //poziva se kad se novi klijent poveže na server

signals:
    void NewClient(Contestant* contestant);                     //signalizira contestmanager-u da se povezao novi takmičar

private:
    QTcpServer* m_server;                                       //unutrašnji server
    QHostAddress m_host;                                        //IP adresa na kojoj se osluškuje
    quint16 m_port;                                             //port na kome se osluškuje
    qint32 m_counter;                                           //brojač koji daje korisnicima jedinstven ID
};



#endif // SERVER_H
