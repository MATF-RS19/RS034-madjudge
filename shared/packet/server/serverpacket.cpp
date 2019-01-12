#include "serverpacket.h"
#include "skeepalive.h"
#include "sloginre.h"
#include "scontestapplyre.h"
#include "scontestinfo.h"
#include "sproblemset.h"
#include "sleavere.h"
#include "supdateuserscore.h"
#include "suserleavecontest.h"
#include "sproblemsolutionstatus.h"
#include "scontestfinished.h"

#ifdef CLIENT
ServerPacket* ServerPacket::Decode(DataStream &ds, PacketError &error)
{
    ServerPacket* returnPkt = nullptr;
    EServerPacketType type = ds.Read<EServerPacketType>();
    switch (type)
    {
    case EServerPacketType::KeepAlive:
        returnPkt = new SKeepAlive();
        break;
    case EServerPacketType::LoginResult:
        returnPkt = new SLoginRe();
        break;
    case EServerPacketType::ContestApplyResult:
        returnPkt = new SContestApplyRe();
        break;
    case EServerPacketType::ContestInfo:
        returnPkt = new SContestInfo();
        break;
    case EServerPacketType::ProblemSet:
        returnPkt = new SProblemSet();
        break;
    case EServerPacketType::LeaveResult:
        returnPkt = new SLeaveRe();
        break;
    case EServerPacketType::UpdateUserScore:
        returnPkt = new SUpdateUserScore();
        break;
    case EServerPacketType::UserLeaveContest:
        returnPkt = new SUserLeaveContest();
        break;
    case EServerPacketType::ProblemSolutionStatus:
        returnPkt = new SProblemSolutionStatus();
        break;
    case EServerPacketType::ContestFinished:
        returnPkt = new SContestFinished();
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
        ds.Seek(-static_cast<qint32>(sizeof(EServerPacketType)), DataStream::Current);
        return nullptr;
    }
    returnPkt->SetDs(packetDs);
    return returnPkt;
}
#endif

qint32 ServerPacket::GetType()
{
    return m_type;
}
