#ifndef SOLUTIONMANAGER_H
#define SOLUTIONMANAGER_H

#include <QObject>

class SolutionManager : public QObject
{
    Q_OBJECT
public:
    explicit SolutionManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SOLUTIONMANAGER_H