#include "client.h"

Client::Client(const QString &host, quint16 port)
{
    socket =new QTcpSocket(this);
    socket->connectToHost(host,port);
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()));
}

Client::~Client()
{
}

void Client::sendToServer(QString t)
{
    QTextStream os(socket);
    os<<t;
}
void Client::closeConnection()
{
    socket->close();
    if ( socket->state() == QAbstractSocket::UnconnectedState  ) {
        connect( socket, SIGNAL(delayedCloseFinished()),SLOT(socketClosed()) );
    }
    else
    {
      //  socketClosed();
    }
}

void Client::socketReadyRead()
{
    while ( socket->canReadLine() ) {
     task=socket->readLine();
     qDebug().noquote()<<task;
     std::cout<<"I'm working"<<std::endl;
    }
    std::cout<<"testiram konekciju"<<std::endl;

}
