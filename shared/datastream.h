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
    T Read()                                                                                //implementation in header because of template
    {
        T returnValue;
        ReadRawData(&returnValue, sizeof(T));
        return returnValue;
    }
    QByteArray ReadPackedBytes();
    QByteArray ReadBytes(qint32 size);
    QByteArray ReadToEnd();
    QByteArray ReadAll() const;
    QByteArray ReadAllCompressed() const;                                                   //don't use for now
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
    DataStream& Reset();
    DataStream& Clear();

private:
    QByteArray m_buffer;
    qint32 m_position;
    qint32 m_length;
    QIODevice::OpenMode m_openMode;
    EDataStreamError m_error;

    DataStream& ReadRawData(void* data, const qint32 size);
    DataStream& WriteRawData(const void* data, const qint32 size);

};

#endif // PACKETSTREAM_H
