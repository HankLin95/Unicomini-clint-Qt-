#ifndef BUY_FUNC_H
#define BUY_FUNC_H

#include <QWidget>
#include "mainfunc.h"
#include "func.h"
class Mainfunc;
/*买电子卡功能类*/
class buy_func : public func
{
    Q_OBJECT
public:
    buy_func( Mainfunc *mainfunc);
    void init_layout();
private:

    Mainfunc *mainfunc;
    bool buy_has_card(int money_num,int card_num);
    void buy_print_card(int money_num,int card_num);//打印卡密码
    int buy_add_card(int money_num,int card_num);

    QLabel *buy_money_num_label;//买卡金额
    QLabel *buy_card_num_choose_label;//数量
    QComboBox *buy_money_num_combobox;//金额
    QComboBox *buy_card_num_choose_combobox;//数量
    QPushButton *buy_confirm_pushbutton;//确定购买
    QPushButton *buy_cancel_buy_pushbutton;//取消购买
    QLabel *buy_password_label;//密码
    QLabel *buy_card_num_label;//数量
    QLineEdit *buy_password_edit;//密码
    QLineEdit *buy_card_num_edit;//数量
    QPushButton *buy_print_pushbutton;//打印凭条
    QPushButton*buy_cancel_print_pushbutton;//取消打印
    QString *pwd;
    QDialog* buy_succeed_dia;
signals:
    
public slots:
    void push_confirm_slot();
    void press_cancel_slot();
    void push_print_slot();//将密码打印入文件

};

#endif // BUY_FUNC_H
