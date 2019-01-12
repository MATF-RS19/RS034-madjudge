#include "contestant.h"
#include "../shared/packet/client/ckeepalive.h"
#include "../shared/packet/server/skeepalive.h"
#include "../shared/packet/server/sloginre.h"
#include "../shared/packet/server/scontestapplyre.h"
#include "../shared/packet/server/sproblemsolutionstatus.h"
#include "contest/contestmanager.h"
#include "contest/contest.h"
#include "contest/solutionmanager.h"

Contestant::Contestant(int ID, Client *netClient) : m_ID(ID), m_netClient(netClient), m_loggedIn(false), m_dataHandler(this)
{
    m_context = EUserContext::PromptName;
    m_keepAliveTimer.setInterval(40000);
    connect(&m_keepAliveTimer, SIGNAL(timeout()), this, SLOT(OnKeepAliveTimeout()));
    connect(&m_dataHandler, SIGNAL(Error()), this, SLOT(OnError()));
    connect(m_netClient, SIGNAL(Error()), this, SLOT(OnError()));
    connect(m_netClient, SIGNAL(DataReceived(const QByteArray&)), this, SLOT(OnDataReceived(const QByteArray&)));
    connect(m_netClient, SIGNAL(Disconnected()), this, SIGNAL(Disconnected()));
}

Contestant::~Contestant()
{
    qDebug() << "Contestant deleted.";
    m_netClient->deleteLater();
}

qint32 Contestant::GetID() const
{
    return m_ID;
}

const QString &Contestant::GetName() const
{
    return m_name;
}

bool Contestant::WasLoggedIn()
{
    return m_loggedIn;
}

void Contestant::SetName(const QString &name)
{
    SLoginRe* loginResultPacket;
    if (m_context == EUserContext::PromptName)
    {
        m_name = name;
        if (ContestManager::Get()->UserLogin(this))
        {
            m_loggedIn = true;
            loginResultPacket = new SLoginRe(true);
            m_context = EUserContext::Menu;
            emit LoggedIn();
        }
        else
        {
            loginResultPacket = new SLoginRe(false, "A user with this name already exists. Please choose a different name.");
        }
    }
    else
    {
        loginResultPacket = new SLoginRe(false, "Wrong context");
    }
    SendPacket(loginResultPacket);
}

void Contestant::KeepAlive(time_t timestamp)
{
    if (m_context != EUserContext::Unconnected)
    {
        m_keepAliveTimer.start();
        SKeepAlive* keepAliveRe = new SKeepAlive(timestamp);
        SendPacket(keepAliveRe);
    }
}

void Contestant::ApplyForContest()
{
    SContestApplyRe* contestApplyResultPacket;
    if (m_context == EUserContext::Menu)
    {
        ContestManager* contestMgr = ContestManager::Get();
        if (contestMgr != nullptr)
        {
            contestMgr->EnqueueUser(this);
            m_context = EUserContext::InQueue;
            contestApplyResultPacket = new SContestApplyRe(true);
            qDebug() << "User" << GetName() << "apply for contest successfully";
        }
        else
        {
            contestApplyResultPacket = new SContestApplyRe(false, "Internal server error");
        }
    }
    else
    {
        contestApplyResultPacket = new SContestApplyRe(false, "Wrong context");
    }
    SendPacket(contestApplyResultPacket);
}

void Contestant::ApplyForExercise()
{
    SContestApplyRe* contestApplyResultPacket;
    if (m_context == EUserContext::Menu)
    {
        ContestManager* contestMgr = ContestManager::Get();
        if (contestMgr != nullptr)
        {
            contestMgr->MakeSoloContest(this);
            m_context = EUserContext::InQueue;
            contestApplyResultPacket = new SContestApplyRe(true);
            qDebug() << "User" << GetName() << "apply for solo contest successfully";
        }
        else
        {
            contestApplyResultPacket = new SContestApplyRe(false, "Internal server error");
        }
    }
    else
    {
        contestApplyResultPacket = new SContestApplyRe(false, "Wrong context");
    }
    SendPacket(contestApplyResultPacket);
}

void Contestant::VerifySolution(qint32 problemId, const QString& text)
{
    if (m_context == EUserContext::InContest && !m_verifyingSolution)
    {
        if (m_currentContest != nullptr)
        {
            SolutionManager* mgr = SolutionManager::Get();
            if (mgr != nullptr)
            {
                m_verifyingSolution = true;
                mgr->CheckSolution(text, this, m_currentContest->GetProblem(problemId));
            }
        }
    }
}

void Contestant::LeaveContest()
{
    if (m_context == EUserContext::InContest || m_context == EUserContext::Waiting)
    {
        if (m_currentContest != nullptr)
        {
            qDebug() << "User leave contest";
            m_currentContest->KickUser(this);
            m_currentContest = nullptr;
        }
    }
}

void Contestant::UpdateSolutionStatus(ProblemEntry::ESolutionStatus status)
{
    if (m_context == EUserContext::InContest && m_verifyingSolution)
    {
        SProblemSolutionStatus* statusPacket = new SProblemSolutionStatus(status);
        SendPacket(statusPacket);

        if (status == ProblemEntry::ESolutionStatus::Solved)
        {
            if (m_currentContest != nullptr)
            {
                m_currentContest->UpdateScore(this, 1);
            }
        }
        if(status == ProblemEntry::ESolutionStatus::Solved
        || status == ProblemEntry::ESolutionStatus::WrongAnswer
        || status == ProblemEntry::ESolutionStatus::TimeLimitExceeded
        || status == ProblemEntry::ESolutionStatus::MemoryLimitExceeded
        || status == ProblemEntry::ESolutionStatus::CompilationError)
        {
            m_verifyingSolution = false;
        }
    }
}

void Contestant::Disconnect()
{
    if (m_context != EUserContext::Unconnected)
    {
        ContestManager* contestMgr;
        switch (m_context)
        {
        case EUserContext::InQueue:
            contestMgr = ContestManager::Get();
            if (contestMgr != nullptr)
            {
                contestMgr->RemoveUserFromQueue(this);
            }
            break;
        case EUserContext::InContest:
        case EUserContext::Waiting:
            LeaveContest();
            break;
        default:
            break;
        }
        m_context = EUserContext::Unconnected;
    }
}

void Contestant::SetContest(Contest *contest)
{
    if (m_context == EUserContext::InQueue)
    {
        m_currentContest = contest;
        m_context = EUserContext::Waiting;
    }
}

void Contestant::StartContest()
{
    if (m_context == EUserContext::Waiting)
    {
        m_context = EUserContext::InContest;
    }
}

void Contestant::FinishContest()
{
    if (m_context == EUserContext::InContest)
    {
        m_context = EUserContext::Menu;
        m_currentContest = nullptr;
    }
}

bool Contestant::CanUpdateScore(qint32 problemId)
{
    if (m_solvedProblems.contains(problemId))
    {
        return false;
    }
    m_solvedProblems.insert(problemId);
    return true;
}

void Contestant::OnKeepAliveTimeout()
{
    m_keepAliveTimer.stop();
    Disconnect();
}

void Contestant::SendPacket(Packet *packet, bool del) const
{
    if (m_context != EUserContext::Unconnected)
    {
        Packet::Send(m_netClient, packet);
    }
    if (del)
    {
        delete packet;
    }
}

void Contestant::OnDataReceived(const QByteArray &data)
{
    m_dataHandler.HandleData(data);
}

void Contestant::OnError()
{
    Disconnect();
}

/*void Contestant::run()
{
    qDebug() << socketDescriptor << " Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug()<<socketDescriptor<<" client connected";

    exec();
}*/

/*void Contestant::readyRead()
{
    auto Data=socket->readAll();
    qDebug().noquote()<<socketDescriptor<<" Data in: "<< Data;
    socket->write("testiranje1\ntestiranje2\ntestiranje3\n");
    QString ime(QString::fromStdString(std::tom_string(socketDescriptor)+".c"));
    qDebug()<<ime;
    QFile file( ime );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << Data << endl;
        file.close();
        std::string s="gcc "+QString::number(socketDescriptor).toStdString()+".c -o "+QString::number(socketDescriptor).toStdString()+".out -Wall";
        qDebug()<<s.cm_str();
        system(s.cm_str());
        system(("./"+QString::number(socketDescriptor).toStdString()+".out").cm_str());
    }else{
        qDebug()<<"Nije otvorena datoteka";
    }
    system(("rm "+QString::number(socketDescriptor).toStdString()+".*").cm_str());
}

void Contestant::disconnected()
{
    qDebug()<<socketDescriptor<<" disconnected";
    socket->deleteLater();
    exit(0);
}*/
