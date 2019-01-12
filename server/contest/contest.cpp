#include "contest.h"
#include <contestant.h>
#include "problemsetmanager.h"
#include "../shared/problementry.h"
#include "../shared/packet/server/scontestinfo.h"
#include "../shared/packet/server/sproblemset.h"
#include "../shared/packet/server/sleavere.h"
#include "../shared/packet/server/suserleavecontest.h"
#include "../shared/packet/server/supdateuserscore.h"
#include "../shared/packet/server/scontestfinished.h"
#include "../shared/packet/packet.h"

Contest::Contest(QSet<Contestant *> contestants) : m_waitingTime(10000), m_contestDuration(3600000), m_numberOfProblems(3)
{
    qDebug() << "New contest, contestants:";
    for (const auto& contestant : contestants)
    {
        m_contestants.insert(contestant, 0);
        qDebug() << contestant->GetID() << contestant->GetName();
    }
    m_currentPhase = EContestPhase::Idle;
    m_contestTimer.setInterval(1000);
    connect(&m_contestTimer, SIGNAL(timeout()), this, SLOT(OnPhaseFinished()));
    m_contestTimer.start();
}

Contest::~Contest()
{
    qDebug() << "Contest deleted";
}

void Contest::Start()
{
    SendContestInfo();
}

void Contest::SendContestInfo() const
{
    if (m_currentPhase == EContestPhase::Idle)
    {
        SContestInfo* contestInfoPacket = new SContestInfo(m_waitingTime, m_contestDuration, m_numberOfProblems, m_contestants.keys());
        SendToAll(contestInfoPacket);
    }
}

void Contest::GenerateProblemset()
{
    ProblemSetManager* mgr = ProblemSetManager::Get();
    if (mgr != nullptr)
    {
        m_problemset = mgr->NextRandomSet(m_numberOfProblems);
        SProblemSet* problemSetPacket = new SProblemSet(m_problemset.values());
        SendToAll(problemSetPacket);
    }
}

void Contest::KickUser(Contestant *contestant)
{
    if (m_contestants.remove(contestant) == 1)
    {
        SLeaveRe* leaveResultPacket = new SLeaveRe(true);
        SUserLeaveContest* userLeavePacket = new SUserLeaveContest(contestant->GetID());
        contestant->SendPacket(leaveResultPacket);
        SendToAll(userLeavePacket);
    }
    if (m_contestants.size() == 0)
    {
        qDebug() << "All contestants left, finish contest";
        m_contestTimer.stop();
        m_currentPhase = EContestPhase::Finished;
        emit ContestFinished();
    }
}

void Contest::UpdateScore(Contestant *contestant, qint32 problemId)
{
    if (m_currentPhase == EContestPhase::InProgress)
    {
        if (contestant->CanUpdateScore(problemId))
        {
            m_contestants[contestant] += 1;
            SUpdateUserScore* updateScorePacket = new SUpdateUserScore(contestant->GetID(), m_contestants[contestant]);
            SendToAll(updateScorePacket);
        }
    }
}

void Contest::FinishContest()
{
    if (m_currentPhase == EContestPhase::Finished)
    {
        SContestFinished* contestFinishedPacket = new SContestFinished();
        SendToAll(contestFinishedPacket);
    }
}

ProblemEntry *Contest::GetProblem(qint32 ID) const
{
    auto find = m_problemset.find(ID);
    if (find == m_problemset.end())
    {
        return nullptr;
    }
    return find.value();
}

void Contest::OnPhaseFinished()
{
    qDebug() << "Contest phase finished: " << m_currentPhase;
    EContestPhase nextPhase;
    qint32 nextInterval;
    switch (m_currentPhase)
    {
    case EContestPhase::Idle:
        Start();
        for (auto contestant : m_contestants.keys())
        {
            contestant->SetContest(this);
        }
        nextInterval = m_waitingTime;
        nextPhase = EContestPhase::Waiting;
        break;
    case EContestPhase::Waiting:
        GenerateProblemset();
        for (auto contestant : m_contestants.keys())
        {
            contestant->StartContest();
        }
        nextInterval = m_contestDuration;
        nextPhase = EContestPhase::InProgress;
        break;
    case EContestPhase::InProgress:
        nextInterval = m_waitingTime;
        nextPhase = EContestPhase::Finished;
        break;
    case EContestPhase::Finished:
        FinishContest();
        emit ContestFinished();
        return;
    }
    m_currentPhase = nextPhase;
    m_contestTimer.setInterval(nextInterval);
    m_contestTimer.start();
}

void Contest::SendToAll(Packet *packet) const
{
    for (const auto& contestant : m_contestants.keys())
    {
        contestant->SendPacket(packet, false);
    }
    delete packet;
}
