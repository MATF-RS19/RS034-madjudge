#include "datastream.h"

DataStream::DataStream(QIODevice::OpenMode openMode) : m_openMode(openMode), m_error(EDataStreamError::Ok)
{
    m_position = 0;
    m_length = 0;
}

DataStream::DataStream(const QByteArray &buf, QIODevice::OpenMode openMode) : m_buffer(buf), m_openMode(openMode), m_error(EDataStreamError::Ok)
{
    m_position = 0;
    m_length = buf.size();
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

QByteArray DataStream::ReadToEnd()
{
    return ReadBytes(m_length - m_position);
}

QByteArray DataStream::ReadAll() const
{
    QByteArray returnData(m_length, Qt::Uninitialized);
    memcpy(returnData.data(), m_buffer.data(), static_cast<size_t>(m_length));
    return returnData;
}

QByteArray DataStream::ReadAllCompressed() const
{
    return qCompress(ReadAll());
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
        absPos = m_position + pos;
        break;
    case EDataStreamSeekPosition::End:
        absPos = m_length - pos;
        break;
    }

    if (absPos > m_length || absPos < 0)
    {
        m_error = EDataStreamError::SeekBeyondEnd;
    }
    else
    {
        m_position = absPos;
    }
    return *this;
}

bool DataStream::CanRead() const
{
    return m_position < m_length;
}

DataStream::EDataStreamError DataStream::GetError() const
{
    return m_error;
}

DataStream DataStream::ToReadWrite() const
{
    DataStream returnDs;
    returnDs.WriteBytes(ReadAll());
    return returnDs;
}

DataStream &DataStream::Reset()
{
    Seek(0, EDataStreamSeekPosition::Begin);
    return *this;
}

DataStream &DataStream::Clear()
{
    m_position = 0;
    m_length = 0;
    m_buffer.clear();
    return *this;
}


DataStream &DataStream::ReadRawData(void *data, const qint32 size)
{
    if (m_position + size > m_length)
    {
        m_error = EDataStreamError::ReadBeyondEnd;
    }
    else
    {
        memcpy(data, m_buffer.data() + m_position, static_cast<size_t>(size));                        //use memcpy here because it's faster than qt methods
        m_position += size;
    }
    return *this;
}

DataStream &DataStream::WriteRawData(const void *data, const qint32 size)
{
    m_buffer.append(static_cast<const char*>(data), size);
    m_length += size;
    return *this;
}
