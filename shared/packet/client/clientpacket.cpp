#include "clientpacket.h"
#include "clogin.h"
#include "ckeepalive.h"
#include "ccontestapply.h"
#include "cleavecontest.h"
#include "cproblemsolution.h"

#ifdef SERVER
ClientPacket* ClientPacket::Decode(Contestant *user, DataStream &ds, PacketError &error)
{
    ClientPacket* returnPkt = nullptr;
    EClientPacketType type = ds.Read<EClientPacketType>();
    switch (type)
    {
    case EClientPacketType::Login:
        returnPkt = new CLogin();
        break;
    case EClientPacketType::KeepAlive:
        returnPkt = new CKeepAlive();
        break;
    case EClientPacketType::ContestApply:
        returnPkt = new CContestApply();
        break;
    case EClientPacketType::LeaveContest:
        returnPkt = new CLeaveContest();
        break;
    case EClientPacketType::ProblemSolution:
        returnPkt = new CProblemSolution();
        break;
    }
    if (returnPkt == nullptr)
    {
        error = PacketError::UnknownType;
        return nullptr;
    }
    DataStream packetDs(ds.ReadPackedBytes());
    if (ds.GetError() == DataStream::ReadBeyondEnd)
    {
        error = PacketError::SizeMismatch;
        ds.Seek(-static_cast<qint32>(sizeof(EClientPacketType)), DataStream::Current);
        return nullptr;
    }
    returnPkt->SetUser(user);
    returnPkt->SetDs(packetDs);
    return returnPkt;
}
#endif

qint32 ClientPacket::GetType()
{
    return m_type;
}
