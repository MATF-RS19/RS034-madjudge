#ifndef USER_H
#define USER_H

#include <QObject>
#include "../shared/client.h"
#include "../shared/datahandler.h"

class Contest;

class ProblemEntry;

class User : public QObject
{
    Q_OBJECT

    static User* h_Inst;

    static const qint32 K_KEEP_ALIVE_TIME_SEC = 10;     //keep alive paketi se šalju svakih 10 sekundi
    static constexpr auto K_HOST = "127.0.0.1";         //IP adresa servera
    static const quint16 K_PORT = 44505;                 //port servera

    enum EUserContext                                   //usercontext = trenutni mod u kom se nalazi korisnik, koristi se da ne bi dozvolili da se neke stvari dešavaju kad ne treba, npr. ne može korisnik da šalje zadatke ako trenutno ne igra
    {
        Unconnected,                                    //pre nego što se poveže na server
        Connecting,                                     //poslao je zahtev da se poveže ali još uvek nije dobio odgovor
        PromptName,                                     //čeka se da korisnik unese ime
        Menu,                                           //povezan na server ali još uvek nije krenuo da igra, još je u main menu
        InQueue,                                        //čeka da se upari sa drugim igračima protiv kojih će igrati
        Waiting,                                        //čeka početak takmičenja
        InContest,                                      //trenutno igra
    };

public:
    static void Init();                                 //instancira singlton ove klase i njegov klijent
    static void Shutdown();                             //zatvara sve, treba pozvati samo pri izlasku iz programa
    static User* Get();                                 //vraća singlton instancu

    void Connect();                                     //povezuje korisnika sa serverom, kada se poveže biće pozvan slot OnConnected
    void Reconnect();                                   //ako je korisnik bio povezan pa se desila neka greška, ova funkcija će ga ponovo povezati sa serverom
    void Disconnect();                                  //zatvara vezu između korisnika i servera, trebalo bi da se pozove samo kada se izađe iz programa

    Contest* GetCurrentContest();                       //trenutno takmičenje u kojem se korisnik nalazi
    void MakeContest(qint32 waitingTime, qint32 contestDuration, qint32 numberOfProblems, const QMap<qint32, QString> &contestants);    //pravi novo takmičenje kome takmičar pripada
    void StartContest(const QList<ProblemEntry *> problemset);                                                                          //započinje takmičenje
    void EndContest();                                  //završava i briše trenutno takmičenje
    bool IsInContest();                                 //vraća da li se korisnik trenutno nalazi u takmičenju

    //funkcije koje treba da se zovu iz main window kao odgovori na određene akcije korisnika aplikacije
    void Login(const QString& name);                    //šalje zahtev serveru da se klijent uloguje sa ovim imenom
    void ContestApply();                                //prijavljuje se za contest (multiplayer)
    void ExerciseApply();                               //prijavljuje se za solo contest
    void SubmitProblemSolution(qint32 problemId, const QString& text);          //šalje rešenje zadatka na proveru na server
    void LeaveContest();                                //šalje zahtev da korisnik napusti takmičenje

    //odgovori servera na zahteve klijenta
    void ConfirmLogin();
    void ConfirmContestApply();
    void UpdateSolutionStatus(const QString& message);
    void ConfirmLeaveContest();

    //druge stvari koje mogu da stignu od servera
    void KeepAlive(time_t timestamp);                                   //

    void ReportError(const QString& error);

public slots:
    void OnConnected();                                 //uspešno povezan sa serverom
    void OnDataReceived(const QByteArray &data);        //primio podatke od servera
    void OnError();                                     //desila se greška, ne interesuje nas kakve vrste

    void OnKeepAliveUpdate();                           //periodično šalje keep alive i proverava da li je server primio prethodni

signals:                                                //signali se isključivo koriste kako bi se korisnik povezao sa mainwindow, ovi signali se povezuju sa odgovarajućim slotovima u mainwindow kako bi se prikazale određene informacije
    //konekcija
    void Connected();                                   //javljamo main window da se korisnik uspešno povezao sa serverom, trebalo bi da se pojavi dialog koji će da traži od korisnika da unese ime
    void Disconnected();                                //javljamo main window da se veza sa serverom prekinula, trebalo bi da se očisti sve sa glavnog menija i negde napiše da se desio disconnect

    //errors
    void Error(const QString& message);                       //bilo kakva vrsta greške koja nije kritična, npr. prilikom pokušaja da se napusti takmičenje ili prilikom slanja rešenja zadatka

    //odgovori servera
    void LoggedIn();                                    //javljamo main window da se korisnik uspešno prijavio sa imenom koje je uneo, trebalo bi da se skloni prethodni dialog i aktivira glavni meni
    void AppliedForContest();                           //uspešno se prijavio za takmičenje, sad počinje čekanje da se upari sa drugim igračima
    void EnteredContest();                              //ulazi u takmičenje, trebalo bi prikazati menu za takmičenje
    void LeftContest();                                 //javljamo main window da je korisnik uspešno napustio takmičenje i da ga vrati na glavni meni, ne sme da napusti takmičenje dok server to ne odobri

    //u vezi sa samim takmičenjem
    void GotContestInfo(Contest* contest);              //primio informacije u vezi takmičenja, u ovom trenutku bi već trebalo da se nalazi u meniju za takmičenje i da popuni informacije na ekranu vezane za takmičenje
    void GotProblemset(QList<ProblemEntry*> problems);  //primio problem set, završava se faza čekanja, popunjavaju se polja koja opisuju zadatke i dozvoljava se kucanje i slanje zadataka
    void UpdatedContestants();                          //treba da se ažurira lista korisnika

    //u vezi sa rešenjem zadatka
    void SolutionStatusUpdated(const QString& message); //status trenutno poslatog rešenja je ažuriran, treba da se prikaže poruka
    void GotSolutionResult(const QString& message);     //ovo ne znam da li da koristimo jos uvek//primio rezultat poslatog rešenja, treba da se prikaže poruka (trenutno samo poruka, možda možemo više informacija kasnije..., poeni će se nezavisno ažurirati)

private:
    explicit User();
    ~User();

    void InitClient();
    void ReleaseClient();
    bool SwitchContext(EUserContext oldContext, EUserContext newContext);
    bool VerifyContext(EUserContext expectedContext);

    void SendPacket(Packet* packet);

    Client* m_netClient = nullptr;
    EUserContext m_context;
    QString m_username;
    qint32 m_ID;

    DataHandler m_dataHandler;

    Contest* m_currentContest;

    QTimer m_keepAliveTimer;
    time_t m_lastKeepAlive;
};

#endif // USER_H
