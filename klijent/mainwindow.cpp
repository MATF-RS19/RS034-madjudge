#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
MainWindow::MainWindow() :
    ui(new Ui::MainWindow)
{
    m_user = User::Get();
    ui->setupUi(this);

    connect(m_user, SIGNAL(Error(const QString&)), this, SLOT(OnUserError(const QString&)));
    //connect(user, SIGNAL(Connected()), this, SLOT(OnUserConnected()));
    connect(m_user,SIGNAL(AppliedForContest()),this,SLOT(show()));
}


void MainWindow::onbtnClicked(){
    if (!fileIsChosen){
       //cl->sendToServer(ui->textEdit->toPlainText());
    } else{
        QFile data(nameOfFile);
       if (data.open(QFile::ReadOnly)) {
           QTextStream in(&data);
           QString value=in.readAll();
           //cl->sendToServer(value);
       }
    }
}

void MainWindow::onfilebtnClicked(){
    QString file = QFileDialog::getOpenFileName(
            this, "Izaberite fajl");
    if (!file.isEmpty()){
        nameOfFile=file;
        QFileInfo fi(file);
        QString fileName = fi.fileName();
        ui->label_2->setText(fileName);
        fileIsChosen=true;
    }
}

void MainWindow::OnUserError(const QString& error)
{
    QMessageBox::information(this, "Error", error);
}

MainWindow::~MainWindow()
{
    delete ui;
}
