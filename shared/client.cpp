#include "client.h"

#ifdef CLIENT
Client::Client(const QString &host, quint16 port, QObject* parent) : QObject(parent), m_host(host), m_port(port)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(Connected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(Disconnected()));
}
#endif

#ifdef SERVER
Client::Client(QTcpSocket *socket, QObject* parent) : QObject(parent), m_socket(socket)
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(Connected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(Disconnected()));
}
#endif

Client::~Client()
{
    qDebug() << "Client deleted.";
    m_socket->deleteLater();
}

void Client::Connect()
{
    m_socket->connectToHost(m_host, m_port);
}

void Client::Send(const QByteArray &data)
{
    m_socket->write(data);
}

void Client::Disconnect()
{
    if (m_socket->state() == QTcpSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
}

void Client::OnReadyRead()
{
    QByteArray m_data = m_socket->readAll();
    emit DataReceived(m_data);
}

void Client::OnError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit Error();
}
