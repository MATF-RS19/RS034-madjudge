#include "popuplogin.h"
#include "ui_popuplogin.h"

PopupLogin::PopupLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupLogin)
{
    ui->setupUi(this);
    connect(User::Get(),SIGNAL(LoggedIn()),this,SLOT(close()));
    connect(ui->LogInBtn,SIGNAL(clicked(bool)),this,SLOT(onbtnClicked()));
}

PopupLogin::~PopupLogin()
{
    delete ui;
}

void PopupLogin::onbtnClicked()
{
    User::Get()->Login(ui->LogInEdit->text());

}
