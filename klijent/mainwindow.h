#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onbtnClicked();
    void onfilebtnClicked();

private:
    Ui::MainWindow *ui;
    Client *cl;
    bool fileIsChosen=false;
    QString nameOfFile;
};

#endif // MAINWINDOW_H
