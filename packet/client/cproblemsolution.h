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
    qint32 _problemID;
    QString _text;
};

#endif // CPROBLEMSOLUTION_H
