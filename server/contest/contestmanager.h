#ifndef CONTESTMANAGER_H
#define CONTESTMANAGER_H

#include <QObject>
#include <QSet>
#include <QQueue>
#include <QMutex>
#include "contest.h"
#include "contestant.h"
#include "server.h"

class ContestManager : public QObject
{
    Q_OBJECT

    static ContestManager* h_Inst;

    static const qint32 K_USERS_PER_CONTEST = 2;

public:
    static void Init();
    static void Shutdown();
    static ContestManager* Get();

    bool UserLogin(Contestant* user);                       //proverava da li je moguće da se korisnik uloguje sa trenutnim imenom, ako jeste onda ga dodaje u _users
    void UserLogoff(Contestant* user);                      //izloguje korisnika

    void EnqueueUser(Contestant* user);                     //dodaje takmičara u red za čekanje za takmičenje
    void RemoveUserFromQueue(Contestant* user);             //briše takmičara iz reda za čekanje za takmičenje

    void MakeSoloContest(Contestant* user);                 //pravi solo contest sa takmičarom

private slots:
    void OnNewUser(Contestant* user);                       //dešava se kad god se poveže novi korisnik
    void OnUserLogin();                                     //kad se korisnik prijavi sa imenom
    void OnUserDisconnected();                              //kad se veza sa korisnikom prekine

    void OnContestFinished();                               //kad se takmičenje završi
    void OnUserSolutionUpdated(Contestant* contestant, const ProblemEntry::ESolutionStatus status);                           //status rešenja nekog takmičara se ažurira

private:
    ContestManager();
    ~ContestManager();

    void MakeContest();                                      //kada se dovoljan broj igrača nalazi u redu za čekanje, pravi takmičenje sa njima
    void AddContest(Contest* contest);                       //dodaje takmičenje
    void DeleteContest(Contest* contest);                    //briše takmičenje kada se ono završi

    QSet<Contestant*> m_halfUsers;                           //čuva sve takmičare koji su povezani ali nisu ulogovani
    QMap<QString, Contestant*> m_users;                      //čuva sve takmičare koji su trenutno ulogovani, sortirano po imenima da bi mogli lako da proverimo da li postoji korisnik sa postojećim imenom
    QSet<Contest*> m_contests;                               //čuva sva takmičenja koja se trenutno odigravaju, uključujući i exercise
    QQueue<Contestant*> m_userQueue;                         //čuva sve korisnike koji su se prijavili za takmičenje ali još uvek nisu upareni sa drugima

    QMutex m_contestsLock;                                   //ključ za contest set jer se može desiti da se u isto vreme prave dva takmičenja (solo i regular)
    QMutex m_queueLock;                                      //ključ za user queue jer se može desiti da se više igrača odjednom prijavi za takmičenje

    Server* m_server;                                        //sam server
};

#endif // CONTESTMANAGER_H
