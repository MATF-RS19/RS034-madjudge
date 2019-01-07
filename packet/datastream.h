#ifndef PACKETSTREAM_H
#define PACKETSTREAM_H

#include <QDataStream>
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
    T Read()                                                                                //implementation in header because of template
    {
        T returnValue;
        ReadRawData(&returnValue, sizeof(T));
        return returnValue;
    }
    QByteArray ReadPackedBytes();
    QByteArray ReadBytes(qint32 size);
    QByteArray ReadAll() const;
    QString ReadString();

    //Write data - ReadWrite
    template <typename T>
    DataStream& Write(const T& data){ return WriteRawData(&data, sizeof(data)); }           //implementation in header because of template
    DataStream& WritePackedBytes(const QByteArray& data);
    DataStream& WriteBytes(const QByteArray& data);
    DataStream& WriteString(const QString& str);

    //Utility methods
    DataStream& Seek(qint32 pos, EDataStreamSeekPosition from = EDataStreamSeekPosition::Current);
    bool CanRead() const;
    EDataStreamError GetError() const;
    DataStream ToReadWrite() const;

private:
    QByteArray _buffer;
    qint32 _position;
    qint32 _length;
    QIODevice::OpenMode _openMode;
    EDataStreamError _error;

    DataStream& ReadRawData(void* data, const qint32 size);
    DataStream& WriteRawData(const void* data, const qint32 size);

};

#endif // PACKETSTREAM_H
