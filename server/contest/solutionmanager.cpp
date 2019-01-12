#include "solutionmanager.h"
#include <QtConcurrent/QtConcurrent>
#include "judger.h"
#include "contestant.h"
using namespace judge;
SolutionManager* SolutionManager::h_Inst = nullptr;

void SolutionManager::Init()
{
    if (h_Inst == nullptr)
    {
        h_Inst = new SolutionManager();
    }
}

void SolutionManager::Shutdown()
{
    if (h_Inst != nullptr)
    {
        h_Inst->deleteLater();
        h_Inst = nullptr;
    }
}

SolutionManager *SolutionManager::Get()
{
    return h_Inst;
}

void SolutionManager::CheckSolution(const QString &text, Contestant *contestant, ProblemEntry *entry)
{
    //QtConcurrent::run(this, &SolutionManager::InternalCheckSolution, text, contestant, entry);
    SolutionManager::InternalCheckSolution(text, contestant, entry);
}

void SolutionManager::InternalCheckSolution(const QString &text, Contestant *contestant, ProblemEntry *entry)
{
    //ovde treba sve da se proveri
    //treba reportovati za svaku promenu statusa (compiling, running, itd.)
    //text = fajl koji treba da se prevede sa g++
    //contestant = korisnik čije rešenje proveravamo
    //entry = problem koji se rešava
    QString TestCasesDir = entry->GetTestcasesDir();
    QString pathFile = TestCasesDir + "/temp/"+ QString::number(contestant->GetID())+".c";
    QFile qFile(pathFile);
    if (qFile.open(QIODevice::WriteOnly)) {
       QTextStream out(&qFile); out << text;
       qFile.close();
    }
    emit SolutionStatusUpdated(contestant, ProblemEntry::InQueue);
    emit SolutionStatusUpdated(contestant,ProblemEntry::Compiling);
    if (compiler(TestCasesDir,1,contestant->GetID())!=0){
        qDebug()<<"compilation error";
        emit SolutionStatusUpdated(contestant,ProblemEntry::CompilationError);
    } else{
        qDebug()<<"uspelo je";
        emit SolutionStatusUpdated(contestant,ProblemEntry::Running);
        int result=Judger(TestCasesDir,contestant->GetID(),entry->GetMemoryLimit(),entry->GetTimeLimit());
        switch(result){
             case 0:
                 emit SolutionStatusUpdated(contestant,ProblemEntry::Solved);
                 break;
             case 1:
                 emit SolutionStatusUpdated(contestant, ProblemEntry::WrongAnswer);
                 break;
             case 2:
                 emit SolutionStatusUpdated(contestant, ProblemEntry::TimeLimitExceeded);
                 break;
             case 3:
                 emit SolutionStatusUpdated(contestant,ProblemEntry::MemoryLimitExceeded);
                 break;

        }
     }

    //treba proveriti da li je uspela kompilacija, ako jeste onda idemo Running, inače CompilationError i završimo
    //ako je uspela, onda proveravamo svaki test primer input/output, testcases mogu da se dobiju u entry->GetTestcasesDir();
    //input u formatu 1.in, 2.in, 3.in itd.. output u formatu 1.out, 2.out, 3.out itd...
}
