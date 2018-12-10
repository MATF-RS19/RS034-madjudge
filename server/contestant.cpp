#include "contestant.h"

Contestant::Contestant(int ID, QObject *parent):
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void Contestant::run()
{
    qDebug() << socketDescriptor << " Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug()<<socketDescriptor<<" client connected";

    exec();
}

void Contestant::readyRead()
{
    auto Data=socket->readAll();
    qDebug().noquote()<<socketDescriptor<<" Data in: "<< Data;
    socket->write("testiranje1\ntestiranje2\ntestiranje3\n");
    QString ime(QString::fromStdString(std::to_string(socketDescriptor)+".c"));
    qDebug()<<ime;
    QFile file( ime );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << Data << endl;
        file.close();
        std::string s="gcc "+QString::number(socketDescriptor).toStdString()+".c -o "+QString::number(socketDescriptor).toStdString()+".out -Wall";
        qDebug()<<s.c_str();
        system(s.c_str());
        system(("./"+QString::number(socketDescriptor).toStdString()+".out").c_str());
    }else{
        qDebug()<<"Nije otvorena datoteka";
    }
    system(("rm "+QString::number(socketDescriptor).toStdString()+".*").c_str());
}

void Contestant::disconnected()
{
    qDebug()<<socketDescriptor<<" disconnected";
    socket->deleteLater();
    exit(0);
}
