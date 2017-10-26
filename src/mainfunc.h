#ifndef MAINFUNC_H
#define MAINFUNC_H
#define ADMIN_LOGIN 1
#define VISITOR_LOGIN 2

#include <QGroupBox>
#include <QComboBox>
#include <QStringList>
#include "widget.h"
#include <QRegExp>
class Widget;
class Mainfunc : public QWidget
{
    Q_OBJECT
public:
    ~Mainfunc();
    friend class pay_func;
    friend class buy_func;
    friend class setting_fun;
    explicit Mainfunc(Widget *login,int login_type,QWidget *parent = 0);
    /*充值缴费，余额查询，账单查询，详单查询，买电子卡，系统设置*/
    static void move_cursor(QLineEdit* print, int type);
    static void backspace(QLineEdit *print)
    {
        print->backspace();
        print->setFocus();
    }
    QPushButton* pay_pushbutton;
    QPushButton* balance_search_pushbutton;
    QPushButton*  bill_search_pushbutton;
    QPushButton * detail_search_pushbutton;
    QPushButton * buy_card_pushbutton;
    QPushButton*  setting_pushbutton;
    QGroupBox*  screen_group;
    QGroupBox * num_group;
    QVBoxLayout*  screen_vlayout;
    QPushButton*   number_pushbutton[10];
    QPushButton*  back_pushbutton;
    QPushButton*  backspace_pushbutton;
    QPushButton*  up_pushbutton;
    QPushButton*  down_pushbutton;
    QPushButton * left_pushbutton;
    QPushButton * right_pushbutton;
    QVBoxLayout * all_layout;
    Widget* login;
    QVBoxLayout* fun_layout;
    static QLabel *curren_time;
    /*以下属于充值缴费功能*/
  /*********************************************************/
   /* QLabel *pay_telnum_label;
    QLabel *pay_re_telnum_label;
    QLabel *pay_money_num_label;
    QLabel *pay_password_label;
    QLineEdit *pay_telnum_edit;
    QLineEdit *pay_re_telnum_edit;
    QComboBox *pay_money_num_combobox;
    QLineEdit *pay_password_edit;
    QPushButton *pay_confirm_pushbutton;
    QPushButton *pay_cancel_pushbutton;*/

    /*********************************************************/
    /*以下属于买电子卡功能*/
    /*   QLabel *buy_money_num_label;//买卡金额
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
       QPushButton*buy_cancel_print_pushbutton;//取消打印*/

    /*********************************************************/

    /*余额查询功能*/
 /*   QLabel *balance_tel_label;
    QLineEdit* balance_tel_edit;
    QPushButton* balance_confrim_pushbutton;
    QPushButton* balance_cancel_pushbutton;*/

    /*******************************************************************/
    /*系统设置*/
   /* QLabel *setting_ip_label;
    QLabel *setting_port_label;
    QLabel *setting_device_id_label;
    QLabel *setting_soft_version_label;
    QLabel *setting_hard_version_label;

    QLineEdit *setting_ip_edit;
    QLineEdit *setting_port_edit;
    QLineEdit *setting_device_id_edit;
    QLineEdit *setting_soft_version_edit;
    QLineEdit *setting_hard_version_edit;
    QPushButton *setting_confirm_button;
    QPushButton *setting_cancel_button;*/

    /***************************************************************/



private:




    void init(int login_type);

signals:
    
public slots:
    void back_login_slot();//返回到登入界面
    void back_mainfunc_slot();//返回到主界面

    void push_fun_slot();
};

#endif // MAINFUNC_H
