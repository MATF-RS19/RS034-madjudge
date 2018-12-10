#ifndef SERVER_H
#define SERVER_H

#include <QDebug>
#include <QTcpServer>
#include"contestant.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent=0);
    void startServer();
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);
};



#endif // SERVER_H
