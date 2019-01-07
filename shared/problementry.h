#ifndef PROBLEM_H
#define PROBLEM_H

#include <QFile>
#include <QTextStream>

class ProblemEntry
{
public:

    enum ESolutionStatus
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

    qint32 GetID() const;
    qint32 GetTimeLimit() const;
    qint32 GetMemoryLimit() const;
    const QString& GetDescription() const;

#ifdef SERVER
    const QString& GetTestcasesDir() const;
    bool LoadProblemDescription();
#endif

private:
    qint32 m_ID;

#ifdef SERVER
    QString m_filePath;
    QString m_testCasesPath;
#endif

    qint32 m_timeLimitMs;
    qint32 m_memoryLimitMb;
    QString m_description;
};

#endif // PROBLEM_H
