#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"

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

    void OnUserError(const QString &error);

private:
    Ui::MainWindow *ui;
    bool fileIsChosen=false;
    QString nameOfFile;
    User* m_user;
};

#endif // MAINWINDOW_H
