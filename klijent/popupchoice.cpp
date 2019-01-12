#include "popupchoice.h"
#include "ui_popupchoice.h"

PopupChoice::PopupChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupChoice)
{
    ui->setupUi(this);
    connect(User::Get(),SIGNAL(LoggedIn()),this,SLOT(show()));
    connect(User::Get(),SIGNAL(EnteredContest()),this,SLOT(close()));
    connect(ui->BtnSolo,SIGNAL(clicked(bool)),this,SLOT(onBtnSoloClick()));
    connect(ui->BtnContest,SIGNAL(clicked(bool)),this,SLOT(onBtnContestClick()));
    connect(User::Get(), SIGNAL(AppliedForContest()),this,SLOT(onApplied()));
}

PopupChoice::~PopupChoice()
{
    delete ui;
}

void PopupChoice::onBtnSoloClick()
{
    User::Get()->ExerciseApply();
}

void PopupChoice::onBtnContestClick()
{
    User::Get()->ContestApply();
}

void PopupChoice::onApplied()
{
    ui->LbInfo->setText("Waiting...");
}
