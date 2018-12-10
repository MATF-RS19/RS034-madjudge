#ifndef CLIENT_H
#define CLIENT_H

#include<QTcpSocket>
#include<QDebug>
#include<QString>
#include<iostream>


class Client : public QTcpSocket
{

    Q_OBJECT

public:
    explicit Client(const QString &host, quint16 port);
    ~Client();


private slots:
    void closeConnection();
public:
    void sendToServer(QString t);
private slots:
    void socketReadyRead();

private:
    QTcpSocket *socket;
    QString task;
};

#endif // CLIENT_H
