#include "server.h"

Server::Server(QObject *parent):
    QTcpServer(parent)
{
}

void Server::startServer()
{
    if(!this->listen(QHostAddress::Any,1234))
    {
        qDebug()<<"Server could not started";
    }
    else
    {
        qDebug()<<"Server started";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{

    qDebug() << socketDescriptor << " Connecting...";
    Contestant *contestant=new Contestant(socketDescriptor,this);
    connect(contestant,SIGNAL(finished()),contestant,SLOT(deleteLater()));
    contestant->start();
}
