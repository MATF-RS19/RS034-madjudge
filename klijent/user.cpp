#include "user.h"
#include "../shared/packet/client/clogin.h"
#include "../shared/packet/client/ccontestapply.h"
#include "../shared/packet/client/cproblemsolution.h"
#include "../shared/packet/client/cleavecontest.h"
#include "../shared/packet/client/ckeepalive.h"
#include "contest.h"

User* User::h_Inst = nullptr;

User::User()
{
    m_context = EUserContext::Unconnected;
    m_currentContest = nullptr;
    m_keepAliveTimer.setInterval(K_KEEP_ALIVE_TIME_SEC * 1000);
}

User::~User()
{
    ReleaseClient();
    if (m_currentContest != nullptr)
    {
        delete m_currentContest;
    }
}

void User::InitClient()
{
    if (m_netClient != nullptr)
    {
        ReleaseClient();            //ako klijent već postoji, moramo da ga obrišemo inače će doći do curenja memorije. on može već da postoji jer će se ova funkcija pozivati i kod reconnect
    }
    m_netClient = new Client(K_HOST, K_PORT);
    connect(m_netClient, SIGNAL(Connected()), this, SLOT(OnConnected()));
    connect(m_netClient, SIGNAL(Error()), this, SLOT(OnError()));
    connect(m_netClient, SIGNAL(DataReceived(QByteArray)), this, SLOT(OnDataReceived(QByteArray)));
    connect(&m_keepAliveTimer, SIGNAL(timeout()), this, SLOT(OnKeepAliveUpdate()));
}

void User::ReleaseClient()
{
    m_keepAliveTimer.stop();
    disconnect(&m_keepAliveTimer, SIGNAL(timeout()), this, SLOT(OnKeepAliveUpdate()));
    disconnect(m_netClient, SIGNAL(Connected()), this, SLOT(OnConnected()));
    disconnect(m_netClient, SIGNAL(Error()), this, SLOT(OnError()));
    disconnect(m_netClient, SIGNAL(DataReceived(QByteArray)), this, SLOT(OnDataReceived(QByteArray)));
    m_netClient->deleteLater();
    m_netClient = nullptr;
}

bool User::SwitchContext(User::EUserContext oldContext, User::EUserContext newContext)
{
    if (VerifyContext(oldContext))
    {
        m_context = newContext;
        return true;
    }
    else
    {
        return false;
    }
}

bool User::VerifyContext(User::EUserContext expectedContext)
{
    if (m_context != expectedContext)
    {
        qDebug() << "Context error: expected" << expectedContext << ", got" << m_context;
        return false;
    }
    return true;
}

void User::SendPacket(Packet *packet)
{
    Packet::Send(m_netClient, packet);
    delete packet;
}

void User::Init()
{
    if (h_Inst == nullptr)
    {
        h_Inst = new User();
        h_Inst->InitClient();
    }
}

void User::Shutdown()
{
    if (h_Inst != nullptr)
    {
        h_Inst->deleteLater();
        h_Inst = nullptr;
    }
}

User *User::Get()
{
    return h_Inst;
}

void User::Connect()
{
    if (m_netClient != nullptr)
    {
        m_context = EUserContext::Connecting;
        m_netClient->Connect();
    }
}

void User::Reconnect()
{
    InitClient();
    Connect();
}

void User::Disconnect()
{
    ReleaseClient();
    emit Disconnected();
}

Contest *User::GetCurrentContest()
{
    return m_currentContest;
}

void User::MakeContest(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QMap<qint32, QString>& contestants)
{
    if (m_currentContest == nullptr && SwitchContext(InQueue, Waiting))
    {
         qDebug() << "Contest info received";
         m_currentContest = new Contest(waitingTime, contestDuration, numberOfProblems, contestants);
         emit GotContestInfo(m_currentContest);
    }
}

void User::StartContest(const QList<ProblemEntry*> problemset)
{
    if (m_currentContest != nullptr && SwitchContext(Waiting, InContest))
    {
        qDebug() << "Problemset received, contest started";
        m_currentContest->SetProblemset(problemset);
        emit GotProblemset(problemset);
    }
}

void User::EndContest()
{
    if (m_currentContest != nullptr)
    {
        delete m_currentContest;
        m_currentContest = nullptr;
        emit LeftContest();
    }
}

bool User::IsInContest()
{
    return m_currentContest != nullptr && m_context == EUserContext::InContest;
}

void User::Login(const QString &name)
{
    if (VerifyContext(PromptName))
    {
        CLogin* loginPacket = new CLogin(name);
        SendPacket(loginPacket);
        m_username = name;
    }
}

void User::ContestApply()
{
    if (VerifyContext(Menu))
    {
        CContestApply* contestApplyPacket = new CContestApply(CContestApply::Regular);
        SendPacket(contestApplyPacket);
    }
}

void User::ExerciseApply()
{
    if (VerifyContext(Menu))
    {
        CContestApply* exerciseApplyPacket = new CContestApply(CContestApply::Solo);
        SendPacket(exerciseApplyPacket);
    }
}

void User::SubmitProblemSolution(qint32 problemId, const QString &text)
{
    if (VerifyContext(InContest))
    {
        CProblemSolution* problemSolutionPacket = new CProblemSolution(problemId, text);
        SendPacket(problemSolutionPacket);
    }
}

void User::LeaveContest()
{
    if (VerifyContext(InContest))
    {
        CLeaveContest* leaveContestPacket = new CLeaveContest();
        SendPacket(leaveContestPacket);
    }
}

void User::ConfirmLogin()
{
    if (SwitchContext(PromptName, Menu))
    {
        qDebug() << "Logged in successfully with name" << m_username;
        ContestApply();
        emit LoggedIn();
    }
}

void User::ConfirmContestApply()
{
    if (SwitchContext(Menu, InQueue))
    {
        qDebug() << "Successfully applied for contest/exercise";
        emit AppliedForContest();
    }
}

void User::UpdateSolutionStatus(const QString &message)
{
    if (VerifyContext(InContest))
    {
        emit SolutionStatusUpdated(message);
    }
}

void User::ConfirmLeaveContest()
{
    if (SwitchContext(InContest, Menu))
    {
        EndContest();
        emit LeftContest();
    }
}

void User::KeepAlive(time_t timestamp)
{
    m_lastKeepAlive = timestamp;
}

void User::ReportError(const QString &error)
{
    emit Error(error);
}

void User::OnConnected()
{
    if (SwitchContext(Connecting, PromptName))
    {
        m_keepAliveTimer.start();
        KeepAlive(time(nullptr));
        emit Connected();
        Login("Milos");
    }
}

void User::OnDataReceived(const QByteArray& data)
{
    m_dataHandler.HandleData(data);
}

void User::OnError()
{
    m_context = EUserContext::Unconnected;
    Reconnect();
}

void User::OnKeepAliveUpdate()
{
    time_t currentTime = time(nullptr);
    if (currentTime - m_lastKeepAlive > 3 * K_KEEP_ALIVE_TIME_SEC)
    {
        //predugo je prošlo, računamo da se veza prekinula
        qDebug() << currentTime << m_lastKeepAlive;
        qDebug() << "Connection timed out.";
        Disconnect();
    }
    else
    {
        CKeepAlive* keepAlivePacket = new CKeepAlive(currentTime);
        SendPacket(keepAlivePacket);
        m_keepAliveTimer.start();
    }
}

