#include "mainfunc.h"
#include "bill_func.h"
#include "balance_fun.h"
#include "pay_func.h"
#include "buy_func.h"
#include "timethread.h"
#include "setting_fun.h"
#include "detail_fun.h"
QLabel*  Mainfunc::curren_time=NULL;
Mainfunc::Mainfunc(Widget *login,int login_type,QWidget *parent) :
    QWidget(parent)
{
    this->login=login;
    QIcon icon("./image/icon");
    QPalette background_pal;
    QImage  background_image("./image/back1.jpg");
    this->setWindowIcon(icon);
    background_pal.setBrush(this->backgroundRole(),QBrush(background_image));
    this->setPalette(background_pal);
    QSize background_size(background_image.width(),background_image.height());
    this->setMinimumSize(background_size);
    this->setMaximumSize(background_size);
    this->fun_layout=NULL;
    this->init(login_type);
}
void Mainfunc::init(int login_type)
{
    back_pushbutton=new QPushButton("返回");

    for(int i=0;i<10;i++)
    {
        number_pushbutton[i]=new QPushButton(QString::number(i));
        number_pushbutton[i]->setFocusPolicy(Qt::NoFocus);
    }
    backspace_pushbutton=new QPushButton("删除");
    up_pushbutton=new QPushButton("↑");
    down_pushbutton=new QPushButton("↓");
    left_pushbutton=new QPushButton("←");
    right_pushbutton=new QPushButton("→");
    down_pushbutton->setFocusPolicy(Qt::NoFocus);
    left_pushbutton->setFocusPolicy(Qt::NoFocus);
    right_pushbutton->setFocusPolicy(Qt::NoFocus);
    up_pushbutton->setFocusPolicy(Qt::NoFocus);
    QImage  background_image("./image/back1.jpg");
    QVBoxLayout *vlayout=new QVBoxLayout();
    pay_pushbutton= new QPushButton("充值缴费");
    balance_search_pushbutton=new QPushButton("余额查询");
    bill_search_pushbutton=new QPushButton("账单查询");
    detail_search_pushbutton=new QPushButton("详单查询");
    buy_card_pushbutton=new QPushButton("买电子卡");
    setting_pushbutton=new QPushButton("系统设置");
    QHBoxLayout* screen_hlayout[4];
    screen_hlayout[0]=new QHBoxLayout;
    screen_hlayout[1]=new QHBoxLayout;
    screen_hlayout[2]= new QHBoxLayout;
    screen_hlayout[3]=new QHBoxLayout;
    switch(login_type)
    {
         case ADMIN_LOGIN:
         {
                 break;
         }
        case VISITOR_LOGIN:
        {
                setting_pushbutton->setDisabled(true);
                break;
        }
    }
    curren_time=new QLabel;
    timethread time;
    time.start();
    screen_hlayout[0]->addWidget(pay_pushbutton);
    screen_hlayout[0]->addWidget(balance_search_pushbutton);
    screen_hlayout[1]->addWidget(bill_search_pushbutton);
    screen_hlayout[1]->addWidget(detail_search_pushbutton);
    screen_hlayout[2]->addWidget(buy_card_pushbutton);
    screen_hlayout[2]->addWidget(setting_pushbutton);
    screen_hlayout[3]->addWidget(curren_time);
    QHBoxLayout *num_hlayout[5];
    for(int i=0;i<5;i++)
    {
        num_hlayout[i]=new QHBoxLayout;
    }
    num_hlayout[0]->addWidget(number_pushbutton[7]);
    num_hlayout[0]->addWidget(number_pushbutton[8]);
    num_hlayout[0]->addWidget(number_pushbutton[9]);
    num_hlayout[1]->addWidget(number_pushbutton[4]);
    num_hlayout[1]->addWidget(number_pushbutton[5]);
    num_hlayout[1]->addWidget(number_pushbutton[6]);
    num_hlayout[2]->addWidget(number_pushbutton[1]);
    num_hlayout[2]->addWidget(number_pushbutton[2]);
    num_hlayout[2]->addWidget(number_pushbutton[3]);
    num_hlayout[3]->addWidget(number_pushbutton[0]);
    num_hlayout[3]->addWidget(up_pushbutton);
    num_hlayout[3]->addWidget(backspace_pushbutton);
    num_hlayout[4]->addWidget(left_pushbutton);
    num_hlayout[4]->addWidget(down_pushbutton);
    num_hlayout[4]->addWidget(right_pushbutton);
    screen_vlayout=new QVBoxLayout();
    curren_time->setMaximumHeight(8);
    screen_vlayout->addLayout(screen_hlayout[3]);
    screen_vlayout->addLayout(screen_hlayout[0]);
    screen_vlayout->addLayout(screen_hlayout[1]);
    screen_vlayout->addLayout(screen_hlayout[2]);
     vlayout->addLayout(num_hlayout[0]);
     vlayout->addLayout(num_hlayout[1]);
     vlayout->addLayout(num_hlayout[2]);
     vlayout->addLayout(num_hlayout[3]);
     vlayout->addLayout(num_hlayout[4]);
     vlayout->addWidget(back_pushbutton);
     screen_group=new QGroupBox("屏幕区");
     screen_group->setLayout(screen_vlayout);
     num_group=new QGroupBox("键盘区");
     num_group->setLayout(vlayout);
     screen_group->setGeometry(0,10,background_image.width(),background_image.height()/2);
     num_group->setGeometry(0,background_image.height()/2,background_image.width(),background_image.height()/2);
     all_layout=new QVBoxLayout(this);
     all_layout->addWidget(screen_group);
     all_layout->addWidget(num_group);
     connect(back_pushbutton,SIGNAL(clicked()),this, SLOT(back_login_slot()));
     connect(buy_card_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
     connect(pay_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
     connect(balance_search_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
     connect(setting_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
     connect(detail_search_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
     connect(bill_search_pushbutton,SIGNAL(clicked()),this,SLOT(push_fun_slot()));
}


Mainfunc::~Mainfunc()
{
}

void Mainfunc::back_login_slot()
{
    this->close();
    login->show();
    login->idpwd_clear();
    login->time_start();
}
void Mainfunc::push_fun_slot()
{
    func *function;
    if(sender()==balance_search_pushbutton)
        function=new balance_fun(this);
    else if(sender()==buy_card_pushbutton)
        function=new buy_func(this);
    else if(sender()==pay_pushbutton)
                function=new pay_func(this);
    else if(sender()==detail_search_pushbutton)
        function=new detail_fun(this);
    else if(sender()==setting_pushbutton)
        function=new setting_fun(this);
    else//bill
        function=new bill_func(this);
    function->init_layout();
    this->screen_group->hide();
    screen_group=new QGroupBox();
    screen_group->setLayout(fun_layout);
    all_layout->removeWidget(num_group);
    all_layout->addWidget(screen_group);
    all_layout->addWidget(num_group);
    disconnect(back_pushbutton,SIGNAL(clicked()),this, SLOT(back_login_slot()));
    function->set_focus();
}
void Mainfunc::move_cursor(QLineEdit*print, int type)//光标向前移动一位
{
    print->setCursorPosition(print->cursorPosition()+type);
}
void Mainfunc::back_mainfunc_slot()
{
    //重新回到主界面，然后将返回按钮和回到登入界面链接
    this->screen_group->close();
     qDebug("22");
    delete fun_layout;
      qDebug("22");
    screen_group=new QGroupBox("屏幕区");
    screen_group->setLayout(screen_vlayout);
    all_layout->removeWidget(num_group);
    all_layout->addWidget(screen_group);
    all_layout->addWidget(num_group);
    disconnect(back_pushbutton,SIGNAL(clicked()),this, SLOT(back_mainfunc_slot()));
    connect(back_pushbutton,SIGNAL(clicked()),this, SLOT(back_login_slot()));
}
