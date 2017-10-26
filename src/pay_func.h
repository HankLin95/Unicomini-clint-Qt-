#ifndef PAY_FUNC_H
#define PAY_FUNC_H

#include <QWidget>
#include <QApplication>
#include "mainfunc.h"
#include <QEvent>
#include <QString>
#include <QTextCursor>//文本光标
#include <QTextEdit>
#include <QLineEdit>
#include "func.h"
class Mainfunc;
class pay_func : public func
{
    Q_OBJECT
public:
    pay_func(Mainfunc *mainfunc);
    void init_layout();
    bool eventFilter(QObject *, QEvent *);
    QLineEdit* print;
    int line;
    void set_focus();
private:
    Mainfunc *mainfunc;
     QLabel *pay_telnum_label;
     QLabel *pay_re_telnum_label;
     QLabel *pay_money_num_label;
     QLabel *pay_password_label;
     QLineEdit *pay_telnum_edit;
     QLineEdit *pay_re_telnum_edit;
     QComboBox *pay_money_num_combobox;
     QLineEdit *pay_password_edit;
     QPushButton *pay_confirm_pushbutton;
     QPushButton *pay_cancel_pushbutton;
    void pay_next_focus(int );
    void pay_font_focus(int );

    int pay_if_pwd_ok(QString pwd,int money);
    void print_record();//打印明细
signals:
public slots:
    void pay_append_lineedit_slot();
    void pay_confirm_down_slot();//按下确定键
    void press_direction_button_slot();//按下方向键
    void press_cancel_button_slot();
    void press_backspace_slot();
};

#endif // PAY_FUNC_H
