#ifndef CCONTESTAPPLY_H
#define CCONTESTAPPLY_H

#include "clientpacket.h"

class CContestApply : public ClientPacket
{
public:
    enum EContestApplyType
    {
        Regular,
        Solo
    };

#ifdef SERVER
    CContestApply();
    void Deserialize();
    void Process();
#endif

#ifdef CLIENT
    CContestApply(EContestApplyType type);
    QByteArray Serialize();
#endif

private:
    EContestApplyType m_type;
};

#endif // CCONTESTAPPLY_H
