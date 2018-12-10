#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cl=new Client("127.0.0.1",1234);
    connect(ui->submitbtn,SIGNAL(clicked(bool)),this,SLOT(onbtnClicked()));
    connect(ui->filebt, SIGNAL(clicked(bool)),this, SLOT(onfilebtnClicked()));
}


void MainWindow::onbtnClicked(){
    if (!fileIsChosen){
       cl->sendToServer(ui->textEdit->toPlainText());
    } else{
        QFile data(nameOfFile);
       if (data.open(QFile::ReadOnly)) {
           QTextStream in(&data);
           QString value=in.readAll();
           cl->sendToServer(value);
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

MainWindow::~MainWindow()
{
    delete ui;
}
