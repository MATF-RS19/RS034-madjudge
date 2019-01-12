#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include "../shared/packet/packet.h"
#include "../shared/client.h"
#include "../shared/datahandler.h"
#include "../shared/problementry.h"

class Contest;

class Contestant : public QObject
{
    Q_OBJECT

    enum EUserContext                                   //usercontext = trenutni mod u kom se nalazi korisnik, koristi se da ne bi dozvolili da se neke stvari dešavaju kad ne treba, npr. ne može korisnik da šalje zadatke ako trenutno ne igra
    {
        Unconnected,                                    //veza prekinuta
        PromptName,                                     //čeka se da korisnik unese ime
        Menu,                                           //povezan na server ali još uvek nije krenuo da igra, još je u main menu
        InQueue,                                        //čeka da se upari sa drugim igračima protiv kojih će igrati
        Waiting,                                        //čeka početak takmičenja
        InContest                                       //trenutno igra
    };

public:
    explicit Contestant(int ID, Client* netClient);
    ~Contestant();

    qint32 GetID() const;                               //getter za ID
    const QString& GetName() const;                     //getter za ime

    bool WasLoggedIn();                                 //proverava da li je ulogovan (tj. da li je postavljeno ime korisnika)

    void SetName(const QString& name);                  //postavlja ime korisnika koje on unese
    void KeepAlive(time_t timestamp);                   //održava konekciju živom

    void ApplyForContest();                             //prijavljuje korisnika za takmičenje
    void ApplyForExercise();                            //prijavljuje korisnika za exercise
    void VerifySolution(qint32 problemId, const QString& text);                              //proverava rešenje zadatka
    void LeaveContest();                                //napušta takmičenje

    void UpdateSolutionStatus(ProblemEntry::ESolutionStatus status);        //šalje korisniku informacije o statusu rešenja

    void Disconnect();                                  //prekida vezu između korisnika i servera

    void SetContest(Contest* contest);                  //postavlja takmičenje kojem takmičara pripada
    void StartContest();                                //daje takmičaru do znanja da mu je počelo takmičenje
    void FinishContest();                               //završava takmičenje u kojem je trenutno takmičar

    bool CanUpdateScore(qint32 problemId);              //proverava da li je korisnik već rešio ovaj problem

    void SendPacket(Packet* packet, bool del = true) const;                    //šalje paket korisniku

private slots:
    void OnDataReceived(const QByteArray& data);        //primljeni podaci, šalje mu client
    void OnError();                                     //negde se desila neka greška od koje ne može da se oporavi, prekida se veza sa serverom

    void OnKeepAliveTimeout();                          //klijent nije poslao keep alive duže od predviđenog vremena, smatramo da je veza prekinuta

signals:
    void LoggedIn();                                    //signaliziramo uspešan login
    void Disconnected();                                //signaliziramo prekid veze

private:
    qint32 m_ID;                                        //jedinstveni ID takmičara
    Client* m_netClient;                                //klijent
    QString m_name;                                     //ime sa kojim se prijavio takmičar
    EUserContext m_context;                             //trenutni kontekst korisnika

    QTimer m_keepAliveTimer;                            //timer koji proverava da li je konekcija aktivna i održava je aktivnom
    Contest* m_currentContest;                          //trenutno takmičenje u kojem se korisnik nalazi

    bool m_loggedIn;                                    //bool koji nam kaže da li je korisnik prijavljen u toku svoje sesije
    bool m_verifyingSolution;                           //bool koji nam kaže da li korisnik trenutno proverava solution

    DataHandler m_dataHandler;                          //data handler za pretvaranje primljenih podataka u pakete

    QSet<qint32> m_solvedProblems;       //problemi koje je korisnik rešio
};
