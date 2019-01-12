#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include "contest.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "../shared/problementry.h"

MainWindow::MainWindow() :
    ui(new Ui::MainWindow)
{
    m_user = User::Get();
    ui->setupUi(this);

    m_countdownTimer.setInterval(1000);

    connect(m_user, SIGNAL(Error(const QString&)), this, SLOT(OnUserError(const QString&)));
    connect(m_user,SIGNAL(EnteredContest()),this,SLOT(show()));
    connect(ui->submitBtn,SIGNAL(clicked(bool)),this,SLOT(onsubmitbtnClicked()));
    connect(ui->nextBtn,SIGNAL(clicked(bool)),this,SLOT(NextTask()));
    connect(ui->previousBtn,SIGNAL(clicked(bool)),this,SLOT(PreviousTask()));
    connect(ui->leaveBtn, SIGNAL(clicked(bool)), this, SLOT(onleavebtnClicked()));
    connect(m_user,SIGNAL(GotProblemset(QList<ProblemEntry*>)),this,SLOT(GotProblemset(QList<ProblemEntry*>)));
    connect(ui->fileBtn, SIGNAL(clicked(bool)), this, SLOT(onfilebtnClicked()));
    connect(&m_countdownTimer, SIGNAL(timeout()), this, SLOT(TimerTickElapsed()));
    connect(m_user, SIGNAL(GotContestInfo(Contest*)), this, SLOT(GotContestInfo(Contest*)));
    connect(m_user, SIGNAL(SolutionStatusUpdated(const QString&)), this, SLOT(SolutionStatusUpdated(const QString&)));
    connect(m_user, SIGNAL(LeftContest()), this, SLOT(close()));
    connect(m_user, SIGNAL(UpdatedContestants()), this, SLOT(UpdateUserlist()));
}

void MainWindow::GotProblemset(QList<ProblemEntry*> problems){
    m_problems = problems;
    m_currentProblem = m_problems.begin();
    ui->problemDescLabel->setText(QString("Problem description (%1/%2)").arg(QString::number(1), QString::number(m_problems.size())));
    ui->taskTextBrowser->setText((**m_currentProblem).GetDescription());
    m_countdownTimer.stop();
    ui->statusLabel->setText("In contest");
    m_secondsRemaining = m_user->GetCurrentContest()->GetContestDuration() / 1000;
    TimerTickElapsed();
    m_countdownTimer.start();
}

void MainWindow::GotContestInfo(Contest *contest)
{
    m_secondsRemaining = contest->GetWaitingTime() / 1000;
    ui->statusLabel->setText("Waiting for problems");
    UpdateUserlist();
    ui->nameListView->setModel(&m_nameModel);
    ui->scoreListView->setModel(&m_scoreModel);
    TimerTickElapsed();
    m_countdownTimer.start();
}

void MainWindow::SolutionStatusUpdated(const QString &message)
{
    ui->solutionLabel->setText(message);
}

void MainWindow::NextTask(){
    if (m_problems.size() == 0)
    {
        return;
    }
    if(++m_currentProblem==m_problems.end()){
        --m_currentProblem;
    }else{qDebug()<<"Sledeci problem";}
    ui->taskTextBrowser->setText((**m_currentProblem).GetDescription());
    ui->problemDescLabel->setText(QString("Problem description (%1/%2)").arg(QString::number(m_currentProblem - m_problems.begin() + 1), QString::number(m_problems.size())));
}
void MainWindow::PreviousTask(){
    if (m_problems.size() == 0)
    {
        return;
    }
    if(m_currentProblem!=m_problems.begin()){qDebug()<<"Prethodni problem";
        --m_currentProblem;
    }
    ui->taskTextBrowser->setText((**m_currentProblem).GetDescription());
    ui->problemDescLabel->setText(QString("Problem description (%1/%2)").arg(QString::number(m_currentProblem - m_problems.begin() + 1), QString::number(m_problems.size())));
}

void MainWindow::TimerTickElapsed()
{
    if (--m_secondsRemaining < 0)
    {
        m_secondsRemaining = 0;
    }
    QString timeString;
    if (m_secondsRemaining >= 3600)
    {
        timeString = QString("%1h %2m %3s").arg(QString::number(m_secondsRemaining / 3600), QString::number((m_secondsRemaining % 3600) / 60), QString::number(m_secondsRemaining % 60));
    }
    else if (m_secondsRemaining >= 60)
    {
        timeString = QString("%1m %2s").arg(QString::number(m_secondsRemaining / 60), QString::number(m_secondsRemaining % 60));
    }
    else {
        timeString = QString("%1s").arg(QString::number(m_secondsRemaining));
    }
    ui->timeLabel->setText(timeString);
}

void MainWindow::UpdateUserlist()
{
    QMap<qint32, QString> userScores = m_user->GetCurrentContest()->GetContestantScores();
    QStringList scores, names;
    for (auto kvp = userScores.cbegin(); kvp != userScores.cend(); kvp++)
    {
        scores.append(QString::number(kvp.key()));
        names.append(kvp.value());
    }
    m_nameModel.setStringList(names);
    m_scoreModel.setStringList(scores);
}

/*void MainWindow::AppliedForContest(){
    qb = new QMessageBox();
    qb->setText("Waiting for problems");
    qb->setWindowTitle("MADJudge");
    qb->show();
    //qb->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}*/

void MainWindow::onsubmitbtnClicked()
{
    if (m_problems.size() == 0)
    {
        return;
    }
    ui->solutionLabel->setText("");
    User::Get()->SubmitProblemSolution((*m_currentProblem)->GetID(),ui->solutionTextBrowser->toPlainText());
}

void MainWindow::onfilebtnClicked(){
    QString fileStr = QFileDialog::getOpenFileName(
            this, "Please choose a file");
    if (!fileStr.isEmpty()){
        QFile file(fileStr);
        if (file.open(QFile::ReadOnly))
        {
            ui->solutionTextBrowser->setText(file.readAll());
            file.close();
        }
    }
}

void MainWindow::onleavebtnClicked()
{
    m_user->LeaveContest();
}

void MainWindow::OnUserError(const QString& error)
{
    QMessageBox::information(this, "Error", error);
    close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

