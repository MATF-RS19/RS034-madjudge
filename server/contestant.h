#ifndef CONTESTANT_H
#define CONTESTANT_H

#include<QThread>
#include<QTcpSocket>
#include<QDebug>
#include<QFile>

class Contestant : public QThread
{
    Q_OBJECT
public:
    explicit Contestant(int ID,QObject *parent=0);
    void run();


signals:
    void error(QTcpSocket::SocketError socketerror);


public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    int socketDescriptor;
};

#endif // CONTESTANT_H
