#include "balance_fun.h"
#include<QRegExp>
#include<QValidator>
balance_fun::balance_fun(Mainfunc*mainfunc)
{
    this->mainfunc=mainfunc;
}
void balance_fun::init_layout()
{
    balance_cancel_pushbutton=new QPushButton("取消");
    balance_confrim_pushbutton=new QPushButton("确定");
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx );
    balance_tel_edit=new QLineEdit;
    balance_tel_edit->setValidator(validator);
    balance_tel_edit->setMaxLength(11);
    balance_tel_label=new QLabel("请输入要查询的手机号码");
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    QHBoxLayout *balance_hlayout[2]={
        new QHBoxLayout,new QHBoxLayout
    };
    balance_hlayout[0]->addWidget(balance_tel_label);
    balance_hlayout[0]->addWidget(balance_tel_edit);
    balance_hlayout[1]->addWidget(balance_confrim_pushbutton);
    balance_hlayout[1]->addWidget(balance_cancel_pushbutton);
    mainfunc->fun_layout->addLayout(balance_hlayout[0]);
    mainfunc->fun_layout->addLayout(balance_hlayout[1]);
    connect(balance_tel_edit,SIGNAL(returnPressed()),this,SLOT(balance_confirm_push_slot()));
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),mainfunc, SLOT(back_mainfunc_slot()));
    connect(balance_cancel_pushbutton,SIGNAL(clicked()),mainfunc,SLOT(back_mainfunc_slot()));
    connect(balance_confrim_pushbutton,SIGNAL(clicked()),this,SLOT(balance_confirm_push_slot()));
    connect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    for(int i=0;i<10;i++)
    {
        connect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(balance_append_lineedit_slot()));
    }
}

void balance_fun::balance_confirm_push_slot()
{
    mainfunc->login->log_write(Request,Balance);
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char sql[100];
    sprintf(sql,"select balan from balance where tel=\'%s\' ",this->balance_tel_edit->text().toLatin1().data());
    db->gettable(sql,&result,col,row);
    if(row!=1)
    {
         QMessageBox::warning(this,"错误","没有找到该号码");
           mainfunc->login->log_write(Failed,Balance);
    }
    else
    {
          mainfunc->login->log_write(Succeed,Balance);
        QString balance("当前余额为"+QString(result[1]));
        QMessageBox::information(this,"余额查询",balance);
        mainfunc->back_mainfunc_slot();
    }
}

void balance_fun::balance_append_lineedit_slot()
{
    int corsor_pos;
    corsor_pos=balance_tel_edit->cursorPosition();
    balance_tel_edit->setText(balance_tel_edit->text().insert(corsor_pos,QString(((QPushButton*)(sender()))->text())));
    balance_tel_edit->setCursorPosition(corsor_pos+1);
}

void balance_fun::press_direction_button_slot()
{
    QString direc(((QPushButton*)sender())->text());
    if(QString::compare(direc,"←")==0)
       {
           Mainfunc::move_cursor(balance_tel_edit,-1);
       }
       else if(QString::compare(direc,"→")==0)
       {
            Mainfunc::move_cursor(balance_tel_edit,1);
       }
       else
       {
           Mainfunc::backspace(this->balance_tel_edit);
       }
    balance_tel_edit->setFocus();
}
void balance_fun::set_focus()
{
    this->balance_tel_edit->setFocus();
}
