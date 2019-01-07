#include "datastream.h"

DataStream::DataStream(QIODevice::OpenMode openMode) : _buffer(K_INIT_BUFFER_SIZE, Qt::Uninitialized), _openMode(openMode), _error(EDataStreamError::Ok)
{
    _position = 0;
    _length = 0;
}

DataStream::DataStream(const QByteArray &buf, QIODevice::OpenMode openMode) : _buffer(buf), _openMode(openMode), _error(EDataStreamError::Ok)
{
    _position = 0;
    _length = buf.size();
}


QByteArray DataStream::ReadPackedBytes()
{
    qint32 size = Read<qint32>();
    return ReadBytes(size);
}

QByteArray DataStream::ReadBytes(qint32 size)
{
    QByteArray returnData(size, Qt::Uninitialized);
    ReadRawData(returnData.data(), size);
    return returnData;
}

QByteArray DataStream::ReadAll() const
{
    QByteArray returnData(_length, Qt::Uninitialized);
    memcpy(returnData.data(), _buffer.data(), static_cast<size_t>(_length));
    return returnData;
}

QString DataStream::ReadString()
{
    qint32 stringLength = Read<qint32>();
    QByteArray stringBytes = ReadBytes(stringLength);
    return QString::fromUtf8(stringBytes);
}


DataStream &DataStream::WritePackedBytes(const QByteArray &data)
{
    Write(data.size());
    return WriteBytes(data);
}

DataStream &DataStream::WriteBytes(const QByteArray& data)
{
    return WriteRawData(data.data(), data.size());
}

DataStream &DataStream::WriteString(const QString &str)
{
    Write(str.size());
    return WriteBytes(str.toUtf8());
}


DataStream &DataStream::Seek(qint32 pos, EDataStreamSeekPosition from)
{
    qint32 absPos = -1;

    switch (from)
    {
    case EDataStreamSeekPosition::Begin:
        absPos = pos;
        break;
    case EDataStreamSeekPosition::Current:
        absPos = _position + pos;
        break;
    case EDataStreamSeekPosition::End:
        absPos = _length - pos;
        break;
    }

    if (absPos > _length || absPos < 0)
    {
        _error = EDataStreamError::SeekBeyondEnd;
    }
    else
    {
        _position = absPos;
    }
    return *this;
}

bool DataStream::CanRead() const
{
    return _position < _length;
}

DataStream::EDataStreamError DataStream::GetError() const
{
    return _error;
}

DataStream DataStream::ToReadWrite() const
{
    DataStream returnDs;
    returnDs.WriteBytes(ReadAll());
    return returnDs;
}


DataStream &DataStream::ReadRawData(void *data, const qint32 size)
{
    if (_position + size > _length)
    {
        _error = EDataStreamError::ReadBeyondEnd;
    }
    else
    {
        memcpy(data, _buffer.data() + _position, static_cast<size_t>(size));                        //use memcpy here because it's much faster than qt methods
        _position += size;
    }
    return *this;
}

DataStream &DataStream::WriteRawData(const void *data, const qint32 size)
{
    _buffer.append(static_cast<const char*>(data), size);
    _length += size;
    return *this;
}
