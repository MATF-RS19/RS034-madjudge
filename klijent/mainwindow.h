#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include <QMessageBox>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    void onsubmitbtnClicked();
    void onfilebtnClicked();
    void onleavebtnClicked();

    void OnUserError(const QString &error);

    void GotProblemset(QList<ProblemEntry *>);
    void GotContestInfo(Contest* contest);
    void SolutionStatusUpdated(const QString& message);
    void UpdateUserlist();

    void NextTask();
    void PreviousTask();

    void TimerTickElapsed();
private:
    Ui::MainWindow *ui;
    bool fileIsChosen=false;
    QString nameOfFile;
    User* m_user;
    QMessageBox* qb;
    QList<ProblemEntry*> m_problems;
    QList<ProblemEntry*>::iterator m_currentProblem;

    QTimer m_countdownTimer;
    qint32 m_secondsRemaining;

    QStringListModel m_nameModel;
    QStringListModel m_scoreModel;
};

#endif // MAINWINDOW_H
