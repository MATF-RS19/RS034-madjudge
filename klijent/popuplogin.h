#ifndef POPUPLOGIN_H
#define POPUPLOGIN_H

#include "user.h"
#include <QWidget>

namespace Ui {
class PopupLogin;
}

class PopupLogin : public QWidget
{
    Q_OBJECT

public:
    explicit PopupLogin(QWidget *parent = 0);
    ~PopupLogin();

private slots:
    void onbtnClicked();

signals:
    void Login();

private:
    Ui::PopupLogin *ui;
};

#endif // POPUPLOGIN_H
