#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onbtnClicked();
    void onfilebtnClicked();

private:
    Ui::MainWindow *ui;
    bool fileIsChosen=false;
    QString nameOfFile;
};

#endif // MAINWINDOW_H
