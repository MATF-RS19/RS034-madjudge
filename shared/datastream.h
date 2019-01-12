#ifndef PACKETSTREAM_H
#define PACKETSTREAM_H

#include <QByteArray>
#include <QIODevice>
#include <stdlib.h>

class DataStream
{
    const static qint32 K_INIT_BUFFER_SIZE = 128;

public:
    enum EDataStreamError
    {
        Ok = 0,
        ReadBeyondEnd,
        WriteAccessError,
        SeekBeyondEnd
    };

    enum EDataStreamSeekPosition
    {
        Begin,
        Current,
        End
    };

    DataStream(QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    DataStream(const QByteArray& buf, QIODevice::OpenMode openMode = QIODevice::ReadOnly);

    //Read data - ReadOnly | ReadWrite
    template<typename T>
    T Read()                                                                                //čita primitivni tip i vraća njegovu vrednost
    {
        T returnValue;
        ReadRawData(&returnValue, sizeof(T));
        return returnValue;
    }
    QByteArray ReadPackedBytes();                                                           //čita zapakovane bajtove (veličina niza bajtova + taj niz)
    QByteArray ReadBytes(qint32 size);                                                      //čita niz bajtova veličine size
    QByteArray ReadToEnd();                                                                 //čita sve bajtove od trenutne pozicije do kraja strema
    QByteArray ReadAll() const;                                                             //čita sve bajtove od početka streama
    QByteArray ReadAllCompressed() const;                                                   //čita kompresovane bajtove
    QString ReadString();                                                                   //čita qstring

    //Write data - ReadWrite
    template <typename T>
    DataStream& Write(const T& data){ return WriteRawData(&data, sizeof(data)); }           //piše primitivni tip
    DataStream& WritePackedBytes(const QByteArray& data);                                   //piše niz bajtova u zapakovanom obliku
    DataStream& WriteBytes(const QByteArray& data);                                         //piše niz bajtova
    DataStream& WriteString(const QString& str);                                            //piše string

    //Utility methods
    DataStream& Seek(qint32 pos, EDataStreamSeekPosition from = EDataStreamSeekPosition::Current);      //pozicionira se
    bool CanRead() const;                                                                   //da li može još uvek da čita, tj. da li je stigao do kraja streama
    EDataStreamError GetError() const;                                                      //vraća poslednju grešku koja se desila na ovom streamu
    DataStream ToReadWrite() const;                                                         //konvertuje datastream u datastream u koji može da se piše (O(n))
    DataStream& Reset();                                                                    //vraća poziciju streama na početak
    DataStream& Clear();                                                                    //postavlja dužinu streama na 0

private:
    QByteArray m_buffer;                                                                    //unutrašnji bafer
    qint32 m_position;                                                                      //trenutna pozicija
    qint32 m_length;                                                                        //trenutna dužina
    QIODevice::OpenMode m_openMode;                                                         //mod
    EDataStreamError m_error;                                                               //poslednja greška

    DataStream& ReadRawData(void* data, const qint32 size);                                 //čita niz bajtova veličine size sa streama u niz data
    DataStream& WriteRawData(const void* data, const qint32 size);                          //piše niz bajtova data veličine size na stream

};

#endif // PACKETSTREAM_H
