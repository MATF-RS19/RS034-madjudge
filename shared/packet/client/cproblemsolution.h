#ifndef CPROBLEMSOLUTION_H
#define CPROBLEMSOLUTION_H

#include "clientpacket.h"

class CProblemSolution : public ClientPacket
{
public:

#ifdef SERVER
    CProblemSolution();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CProblemSolution(qint32 problemID, const QString& text);
    QByteArray Serialize();
#endif

private:
    qint32 m_problemID;
    QString m_text;
};

#endif // CPROBLEMSOLUTION_H
