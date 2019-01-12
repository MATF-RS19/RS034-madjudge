#include "problemsetmanager.h"
#include "solutionmanager.h"

ProblemSetManager* ProblemSetManager::h_Inst = nullptr;

ProblemSetManager::ProblemSetManager(const QString &path) : m_path(path), m_counter(0){}

bool ProblemSetManager::Init(const QString &path)
{
    if (h_Inst == nullptr)
    {
        SolutionManager::Init();
        h_Inst = new ProblemSetManager(QDir::currentPath() + "/" + path);
        return h_Inst->LoadFiles();
    }
    return false;
}

void ProblemSetManager::Shutdown()
{
    if (h_Inst != nullptr)
    {
        for (ProblemEntry* problemEntry : h_Inst->m_problems)
            delete problemEntry;
        h_Inst->m_problems.clear();
        delete h_Inst;
        h_Inst = nullptr;
        SolutionManager::Shutdown();
    }
}

ProblemSetManager *ProblemSetManager::Get()
{
    return h_Inst;
}

QMap<qint32, ProblemEntry *> ProblemSetManager::NextRandomSet(qint32 count)
{
    //order of the elements doesn't matter when picking randomly, so we can leave the problem vector in the state the the algorithm will produce
    m_problemsLock.lock();
    QMap<qint32, ProblemEntry*> returnMap;
    qint32 left = m_problems.size();
    auto begin = m_problems.begin();
    while (count--)
    {
        auto r = begin;
        std::advance(r, rand() % left);
        returnMap.insert((*r)->GetID(), *r);
        std::swap(*r, *begin);
        ++begin;
        --left;
    }
    m_problemsLock.unlock();
    return returnMap;
}

bool ProblemSetManager::LoadFiles()
{
    QDir directory(m_path);
    if (directory.exists())
    {
        QFile configFile(m_path + "/problems.conf");
        if (configFile.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream configFileStream(&configFile);
            while (!configFileStream.atEnd())
            {
                QString entry = configFileStream.readLine().remove(' ');
                QStringList parts = entry.split('|');
                if (parts.length() == 4)
                {
                    bool bTime, bMem;
                    QString problemFilePath = m_path + "/descriptions/" + parts.at(0);
                    qint32 problemTimeLimit = parts.at(1).toInt(&bTime);
                    qint32 problemMemoryLimit = parts.at(2).toInt(&bMem);
                    QString testCasesPath = m_path + "/testcases/" + parts.at(3);
                    if (bTime && bMem && directory.exists(testCasesPath))
                    {
                        ProblemEntry* problemEntry = new ProblemEntry(++m_counter, problemFilePath, testCasesPath, problemTimeLimit, problemMemoryLimit);
                        if (problemEntry->LoadProblemDescription())
                        {
                            m_problems.append(problemEntry);
                        }
                    }
                }
            }
            qDebug() << "Loaded" << m_problems.size() << "problems.";
            return true;
        }
        qDebug() << "Error opening problem set config file" << configFile.fileName() << ":" << configFile.errorString();
        return false;
    }
    qDebug() << "Problem set directory " << m_path << " does not exist.";
    return false;
}
