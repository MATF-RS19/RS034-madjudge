#ifndef PROBLEM_H
#define PROBLEM_H

#include <QFile>
#include <QTextStream>

class ProblemEntry
{
public:

    enum ESolutionStatus                                                //self-explanatory
    {
        InQueue,
        Compiling,
        Running,
        CompilationError,
        WrongAnswer,
        TimeLimitExceeded,
        MemoryLimitExceeded,
        Solved
    };

#ifdef SERVER
    ProblemEntry(qint32 ID, const QString& filePath, const QString& testCasesPath, qint32 timeLimitMs, qint32 memoryLimitMb);
#endif

#ifdef CLIENT
    ProblemEntry(qint32 ID, qint32 timeLimitMs, qint32 memoryLimitMb, const QString& description);
#endif

    //getteri
    qint32 GetID() const;
    qint32 GetTimeLimit() const;
    qint32 GetMemoryLimit() const;
    const QString& GetDescription() const;

#ifdef SERVER
    const QString& GetTestcasesDir() const;
    bool LoadProblemDescription();                                          //učitava opis problema iz fajla na koji pokazuje m_filePath
#endif

private:
    qint32 m_ID;                                                            //ID problema

#ifdef SERVER
    QString m_filePath;                                                     //put do fajla gde se nalazi opis problema
    QString m_testCasesPath;                                                //put do foldera gde se nalaze test primeri za ovaj problem
#endif

    qint32 m_timeLimitMs;                                                   //vremensko ograničenje za ovaj problem
    qint32 m_memoryLimitMb;                                                 //memorijsko ograničenje za ovaj problem
    QString m_description;                                                  //opis ovog problema (biće postavljen tek nakon poziva LoadProblemDescription())
};

#endif // PROBLEM_H
