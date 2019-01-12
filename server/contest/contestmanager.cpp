#include "contestmanager.h"
#include "solutionmanager.h"

ContestManager* ContestManager::h_Inst = nullptr;

void ContestManager::Init()
{
    if (h_Inst == nullptr)
    {
        h_Inst = new ContestManager();
        h_Inst->m_server->Start();
        SolutionManager* mgr = SolutionManager::Get();
        if (mgr != nullptr)
        {
            connect(mgr, SIGNAL(SolutionStatusUpdated(Contestant*, const ProblemEntry::ESolutionStatus)), h_Inst, SLOT(OnUserSolutionUpdated(Contestant*, const ProblemEntry::ESolutionStatus)));
        }
    }
}

void ContestManager::Shutdown()
{
    if (h_Inst != nullptr)
    {
        h_Inst->deleteLater();
        h_Inst = nullptr;
    }
}

ContestManager *ContestManager::Get()
{
    return h_Inst;
}

bool ContestManager::UserLogin(Contestant *user)
{
    if (m_users.contains(user->GetName()))
    {
        return false;
    }
    m_users.insert(user->GetName(), user);
    return true;
}

void ContestManager::UserLogoff(Contestant *user)
{
    m_users.remove(user->GetName());
}

void ContestManager::EnqueueUser(Contestant *user)
{
    m_queueLock.lock();
    m_userQueue.enqueue(user);
    if (m_userQueue.size() == K_USERS_PER_CONTEST)
    {
        MakeContest();
    }
    m_queueLock.unlock();
}

void ContestManager::RemoveUserFromQueue(Contestant *user)
{
    m_queueLock.lock();
    m_userQueue.removeOne(user);
    m_queueLock.unlock();
}

void ContestManager::MakeSoloContest(Contestant *user)
{
    QSet<Contestant*> contestants;
    contestants.insert(user);
    Contest* contest = new Contest(contestants);
    AddContest(contest);
}

void ContestManager::OnNewUser(Contestant *user)
{
    m_halfUsers.insert(user);
    connect(user, SIGNAL(LoggedIn()), this, SLOT(OnUserLogin()));
    connect(user, SIGNAL(Disconnected()), this, SLOT(OnUserDisconnected()));
}

void ContestManager::OnUserLogin()
{
    Contestant* user = dynamic_cast<Contestant*>(sender());
    disconnect(user, SIGNAL(LoggedIn()), this, SLOT(OnUserLogin()));
    m_halfUsers.remove(user);
    m_users.insert(user->GetName(), user);
    qDebug() << "User login with name" << user->GetName();
}

void ContestManager::OnUserDisconnected()
{
    Contestant* user = dynamic_cast<Contestant*>(sender());
    if (user->WasLoggedIn())
    {
        m_users.remove(user->GetName());
    }
    else
    {
        m_halfUsers.remove(user);
    }
    user->deleteLater();
}

void ContestManager::OnContestFinished()
{
    Contest* contest = dynamic_cast<Contest*>(sender());
    DeleteContest(contest);
}

void ContestManager::OnUserSolutionUpdated(Contestant *contestant, const ProblemEntry::ESolutionStatus status)
{
    contestant->UpdateSolutionStatus(status);
}

ContestManager::ContestManager()
{
    m_server = new Server("127.0.0.1", 44505);
    connect(m_server, SIGNAL(NewClient(Contestant*)), this, SLOT(OnNewUser(Contestant*)));
}

ContestManager::~ContestManager()
{
    m_server->deleteLater();
}

void ContestManager::MakeContest()
{
    QSet<Contestant*> contestants;
    for (qint32 i = 0; i < K_USERS_PER_CONTEST; i++)
    {
        contestants.insert(m_userQueue.dequeue());
    }
    Contest* contest = new Contest(contestants);
    AddContest(contest);
}

void ContestManager::AddContest(Contest *contest)
{
    connect(contest, SIGNAL(ContestFinished()), this, SLOT(OnContestFinished()));
    m_contestsLock.lock();
    m_contests.insert(contest);
    m_contestsLock.unlock();
}

void ContestManager::DeleteContest(Contest *contest)
{
    m_contests.remove(contest);
    contest->deleteLater();
}
