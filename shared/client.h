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

    void Connect();
    void Send(const QByteArray &data);
    void Disconnect();

private slots:
    void OnReadyRead();
    void OnError(QAbstractSocket::SocketError error);

signals:
    void Connected();
    void DataReceived(QByteArray data);
    void Disconnected();
    void Error();

private:

    QHostAddress m_host;
    quint16 m_port;
    QTcpSocket* m_socket;
};

#endif // CLIENT_H
