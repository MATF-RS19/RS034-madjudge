#include "problementry.h"

#ifdef SERVER
ProblemEntry::ProblemEntry(qint32 ID, const QString &filePath, const QString &testCasesPath, qint32 timeLimitMs, qint32 memoryLimitMb) : m_ID(ID), m_filePath(filePath), m_testCasesPath(testCasesPath), m_timeLimitMs(timeLimitMs), m_memoryLimitMb(memoryLimitMb){}
#endif

#ifdef CLIENT
ProblemEntry::ProblemEntry(qint32 ID, qint32 timeLimitMs, qint32 memoryLimitMb, const QString& description) : m_ID(ID), m_timeLimitMs(timeLimitMs), m_memoryLimitMb(memoryLimitMb), m_description(description){}
#endif

qint32 ProblemEntry::GetID() const
{
    return m_ID;
}

const QString &ProblemEntry::GetDescription() const
{
    return m_description;
}

qint32 ProblemEntry::GetTimeLimit() const
{
    return m_timeLimitMs;
}

qint32 ProblemEntry::GetMemoryLimit() const
{
    return m_memoryLimitMb;
}

#ifdef SERVER
const QString &ProblemEntry::GetTestcasesDir() const
{
    return m_testCasesPath;
}

bool ProblemEntry::LoadProblemDescription()
{
    QFile problemFile(m_filePath);
    if (problemFile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream fileStream(&problemFile);
        m_description = fileStream.readAll();
        return true;
    }
    return false;
}
#endif
