#include "server.h"

Server::Server(QString host, quint16 port) : m_host(host), m_port(port)
{
    m_server = new QTcpServer();
    connect(m_server, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
}

Server::~Server()
{
    disconnect(m_server, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
    delete m_server;
}

void Server::Start()
{
    if (m_server->listen(m_host, m_port))
    {
        qDebug() << "Server started.";
    }
    else
    {
        qDebug() << "Server could not be started, check if the port " << m_port << " is available.";
    }
}

void Server::OnNewConnection()
{
    QTcpSocket* clientSocket = m_server->nextPendingConnection();
    qDebug() << "New client connected.";
    Client* client = new Client(clientSocket);
    Contestant* contestant = new Contestant(++m_counter, client);
    emit NewClient(contestant);
}
