#ifndef BALANCE_FUN_H
#define BALANCE_FUN_H
#include <QWidget>
#include"mainfunc.h"
#include "func.h"
class Mainfunc;
class balance_fun : public func
{
    Q_OBJECT
public:
    balance_fun(Mainfunc *mainfunc);
    void init_layout();

    void set_focus();

signals:
private:
    QLabel *balance_tel_label;
    QLineEdit* balance_tel_edit;
    QPushButton* balance_confrim_pushbutton;
    QPushButton* balance_cancel_pushbutton;
public slots:
    void balance_confirm_push_slot();
    void balance_append_lineedit_slot();
    void press_direction_button_slot();
};

#endif // BALANCE_FUN_H
