#ifndef POPUPCHOICE_H
#define POPUPCHOICE_H

#include <QWidget>
#include "user.h"

namespace Ui {
class PopupChoice;
}

class PopupChoice : public QWidget
{
    Q_OBJECT

public:
    explicit PopupChoice(QWidget *parent = nullptr);
    ~PopupChoice();

private slots:
    void onBtnSoloClick();
    void onBtnContestClick();
    void onApplied();
private:
    Ui::PopupChoice *ui;
};

#endif // POPUPCHOICE_H
