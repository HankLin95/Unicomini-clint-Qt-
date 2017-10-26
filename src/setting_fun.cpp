#include "pay_func.h"
#include "setting_fun.h"
setting_fun::setting_fun(Mainfunc* mainfunc)
{
    this->mainfunc=mainfunc;

}

void setting_fun::init_layout()
{
    line=1;
    for(int i=0;i<5;i++)
    {
        setting_edit[i]=new QLineEdit;
        setting_edit[i]->installEventFilter(this);
    }
    setting_ip_label=new QLabel("IP");
    setting_port_label=new QLabel("端口");
    setting_device_id_label=new QLabel("设备ID");
    setting_soft_version_label=new QLabel("软件版本");
    setting_hard_version_label=new QLabel("硬件版本");
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    setting_confirm_button=new QPushButton("确定");
    setting_cancel_button=new QPushButton("取消");
    QHBoxLayout* setting_hlayout[6]=
    {new QHBoxLayout,
     new QHBoxLayout,
     new QHBoxLayout,
     new QHBoxLayout,
     new QHBoxLayout,
     new QHBoxLayout
    };
    setting_hlayout[0]->addWidget(setting_ip_label);
    setting_hlayout[0]->addWidget(setting_edit[0]);
    setting_hlayout[1]->addWidget(setting_port_label);
    setting_hlayout[1]->addWidget(setting_edit[1]);
    setting_hlayout[2]->addWidget(setting_device_id_label);
    setting_hlayout[2]->addWidget(setting_edit[2]);
    setting_hlayout[3]->addWidget(setting_soft_version_label);
    setting_hlayout[3]->addWidget(setting_edit[3]);
    setting_hlayout[4]->addWidget(setting_hard_version_label);
    setting_hlayout[4]->addWidget(setting_edit[4]);
    setting_hlayout[5]->addWidget(setting_confirm_button);
    setting_hlayout[5]->addWidget(setting_cancel_button);

    this->add_dbdata();

    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    for(int i=0;i<6;i++)
    {
        setting_hlayout[i]->setSpacing(40);

         mainfunc->fun_layout->addLayout(setting_hlayout[i]);
    }
    print=setting_edit[0];
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
     connect(setting_cancel_button,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    for(int i=0;i<10;i++)
    {
        connect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(setting_append_lineedit_slot()));
    }
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    connect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(setting_confirm_button,SIGNAL(clicked()),this,SLOT(press_confirm_button_slot()));
    connect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
}


void setting_fun::set_focus()
{
    setting_edit[0]->setFocus();
}

bool setting_fun::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress)
    {
        qDebug("获取到鼠标点击信号");
        for(int i=0;i<5;i++)
        {
            if(o==setting_edit[i])
            {
                line=i+1;
                print=setting_edit[i];
                return true;
            }
        }
    }
    return QWidget::eventFilter(o,e);
}
void setting_fun::setting_append_lineedit_slot()
{
        int corsor_pos;
        corsor_pos=print->cursorPosition();  qDebug("sd");
        print->setText(print->text().insert(corsor_pos,QString(((QPushButton*)(sender()))->text())));
        print->setCursorPosition(corsor_pos+1);
        print->setFocus();
}

void setting_fun::press_direction_button_slot()
{
    QString direc(((QPushButton*)sender())->text());//获取按键信息
    if(QString::compare(direc,"↑")==0)//判断按键
    {
            setting_font_focus();
    }
    else if(QString::compare(direc,"↓")==0)
    {
        setting_next_focus();
    }
    else if(QString::compare(direc,"←")==0)
    {
        Mainfunc::move_cursor(print,-1);
    }
    else if(QString::compare(direc,"→")==0)
    {
          Mainfunc::move_cursor(print,1);
    }
    else //删除
    {
        Mainfunc::backspace(print);
    }
    print->setFocus();
}

void setting_fun::press_confirm_button_slot()
{
    this->mainfunc->login->log_write(Request,Setting);
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char sql[120]={0};
    sprintf(sql,"update setting set ip=\'%s\',port=%d,device=\'%s\',soft=\'%s\',hard=\'%s\' where ip=\'%s\'",
            setting_edit[0]->text().toLatin1().data(),setting_edit[1]->text().toInt(),
            setting_edit[2]->text().toLatin1().data(),setting_edit[3]->text().toLatin1().data()
            ,setting_edit[4]->text().toLatin1().data(),pre_ip);
    db->gettable(sql,&result,col,row);
    if(row!=1)
    {
        QMessageBox::information(this,"成功","修改成功");
        this->mainfunc->login->log_write(Succeed,Setting);
    }
    else
    {
        this->mainfunc->login->log_write(Failed,Setting);

    }
    this->press_cancel_slot();
}

void setting_fun::setting_next_focus()
{
    if(line<=4)
    {
        line++;
    }
    else
        line=1;
    print=setting_edit[line-1];
}

void setting_fun::setting_font_focus()
{
    if(line>=2)
    {
        line--;
    }
    else
        line=5;
       print=setting_edit[line-1];
}
void setting_fun::add_dbdata()
{
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char sql[100];
    memset(sql,0,100);
    sprintf(sql,"select* from setting");
    db->gettable(sql,&result,col,row);
    for(int i=0;i<5;i++)
    {
        this->setting_edit[i]->setText(QString(result[i+5]));
    }
    pre_ip=new char[strlen(result[5])+1];
    strcpy(pre_ip,result[5]);
    qDebug(pre_ip);
}
void setting_fun::press_cancel_slot()
{
    for(int i=0;i<10;i++)
    {
        disconnect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(setting_append_lineedit_slot()));
    }
    disconnect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(setting_confirm_button,SIGNAL(clicked()),this,SLOT(press_confirm_button_slot()));
    disconnect(mainfunc->back_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    disconnect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    mainfunc->back_mainfunc_slot();
}


