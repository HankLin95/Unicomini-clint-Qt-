#include "pay_func.h"
pay_func::pay_func(Mainfunc*mainfunc)
{
    this->mainfunc=mainfunc;

    line=1;
}
void pay_func::init_layout()
{
    pay_telnum_label=new QLabel("请输入手机号码");
    pay_re_telnum_label=new QLabel("重新输入手机号码");
   pay_money_num_label=new QLabel("选择充值金额");
    pay_password_label=new QLabel("输入交易密码");
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx );
    pay_telnum_edit=new QLineEdit;
    pay_telnum_edit->setValidator(validator);
    pay_re_telnum_edit=new QLineEdit;
    pay_re_telnum_edit->setValidator(validator);
    pay_password_edit=new QLineEdit;
    pay_password_edit->setValidator(validator);
    pay_password_edit->setEchoMode(QLineEdit::Password);
    QStringList money=(QStringList()<<"30"<<"50"<<"100");
   pay_money_num_combobox=new QComboBox();
    pay_money_num_combobox->addItems(money);
    pay_confirm_pushbutton=new QPushButton("确定");
    pay_cancel_pushbutton=new QPushButton("取消");
    pay_re_telnum_edit->installEventFilter(this);
    pay_re_telnum_edit->setMaxLength(11);
    pay_telnum_edit->installEventFilter(this);
    pay_telnum_edit->setMaxLength(11);
    pay_password_edit->installEventFilter(this);
    QHBoxLayout *pay_hlayout[5];
    for(int i=0;i<5;i++)
    {
        pay_hlayout[i]=new QHBoxLayout;
    }
    pay_hlayout[0]->addWidget(pay_telnum_label);
    pay_hlayout[0]->addWidget(pay_telnum_edit);
    pay_hlayout[1]->addWidget(pay_re_telnum_label);
    pay_hlayout[1]->addWidget(pay_re_telnum_edit);
    pay_hlayout[2]->addWidget(pay_money_num_label);
    pay_hlayout[2]->addWidget(pay_money_num_combobox);
    pay_hlayout[3]->addWidget(pay_password_label);
    pay_hlayout[3]->addWidget(pay_password_edit);
    pay_hlayout[4]->addWidget(pay_confirm_pushbutton);
    pay_hlayout[4]->addWidget(pay_cancel_pushbutton);
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    QImage  background_image("./image/back1.jpg");
    mainfunc->fun_layout->setGeometry(QRect(0,0,background_image.width(),(background_image.height()/2)));
    for(int i=0;i<5;i++)
    {
        mainfunc->fun_layout->addLayout(pay_hlayout[i]);
    }
    for(int i=0;i<10;i++)
    {
        connect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(pay_append_lineedit_slot()));
    }
    connect(pay_telnum_edit,SIGNAL(returnPressed()),pay_re_telnum_edit,SLOT(setFocus()));
    connect(pay_re_telnum_edit,SIGNAL(returnPressed()),pay_password_edit,SLOT(setFocus()));
    connect(pay_password_edit,SIGNAL(returnPressed()),this,SLOT(pay_confirm_down_slot()));
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_button_slot()));
    connect(pay_confirm_pushbutton,SIGNAL(clicked()),this,SLOT(pay_confirm_down_slot()));
    connect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(pay_cancel_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_button_slot()));
    connect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_backspace_slot()));
}

bool pay_func::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress)
    {
        if(o==pay_telnum_edit)
        {
            line=1;
        }
        else if(o==pay_re_telnum_edit)
        {
            line=2;
        }
        else
        {
            line=3;
        }
        print=(QLineEdit*)o;
        print->setCursorPosition(print->text().length());
        return true;
    }
    return QWidget::eventFilter(o,e);
}
void pay_func::set_focus()
{
    pay_telnum_edit->setFocus();
    print=pay_telnum_edit;
}
void pay_func::pay_append_lineedit_slot()
{
        int corsor_pos=print->cursorPosition();
        print->setText(print->text().insert(corsor_pos,QString(((QPushButton*)(sender()))->text())));
        print->setCursorPosition(corsor_pos+1);
      //  print->setFocus();
}

void pay_func::pay_confirm_down_slot()
{
     mainfunc->login->log_write(Request,Pay);
    int money=this->pay_money_num_combobox->itemText(pay_money_num_combobox->currentIndex()).toInt();
    if((QString::compare(pay_re_telnum_edit->text(),pay_telnum_edit->text())==0)&&pay_re_telnum_edit->text().length()<=11)
    {
        if(this->pay_if_pwd_ok(this->pay_password_edit->text(),money))//此处判断是否有合法的密码
        {
            mainfunc->login->log_write(Succeed,Pay);
            QString mes("您为"+pay_telnum_edit->text()+"充值"+pay_money_num_combobox->itemText(pay_money_num_combobox->currentIndex()));
            QMessageBox::information(NULL,"成功",mes,QMessageBox::Ok);
            mainfunc->back_mainfunc_slot();
            //此处将话费加入该号码 并且将这个密码改为已使用
        }
        else
        {
            QMessageBox::warning(NULL,"错误","请输入正确的密码和手机号",QMessageBox::Ok);
            mainfunc->login->log_write(Failed,Pay);
        }
    }
    else
    {
        QMessageBox::warning(NULL,"错误","请输入两次相同的合法手机号",QMessageBox::Ok);
        mainfunc->login->log_write(Failed,Pay);
    }
}

void pay_func::press_direction_button_slot()
{
    QString direc(((QPushButton*)sender())->text());//获取按键信息
    if(QString::compare(direc,"↑")==0)//判断按键
    {
            pay_font_focus(line);
    }
    else if(QString::compare(direc,"↓")==0)
    {
        pay_next_focus(line);
    }
    else if(QString::compare(direc,"←")==0)
    {
        Mainfunc::move_cursor(print,-1);
    }
    else
    {
        Mainfunc::move_cursor(print,1);
    }
//    print->setFocus();
}

void pay_func::press_cancel_button_slot()
{
    delete pay_telnum_edit;
    delete pay_re_telnum_edit;
    delete pay_password_edit;
    delete pay_telnum_label;
    delete pay_re_telnum_label;
    delete  pay_password_label;
    delete pay_money_num_combobox;
    delete pay_money_num_label;
    delete  pay_confirm_pushbutton;
    delete pay_cancel_pushbutton;
    disconnect(mainfunc->back_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_button_slot()));
    disconnect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_backspace_slot()));
    for(int i=0;i<10;i++)
    {
        disconnect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(pay_append_lineedit_slot()));
    }
    mainfunc->back_mainfunc_slot();
}

void pay_func::press_backspace_slot()
{
    print->backspace();
    print->setFocus();
}

void pay_func::pay_next_focus(int local)
{
    switch(local)
    {
    case 1://在输手机号
    {
        print=pay_re_telnum_edit;
        line=2;
        break;
    }
    case 2://第二次输手机号
    {
           print=pay_password_edit;
           line=3;
            break;
    }
    case 3://输入密码
    {
        print=pay_telnum_edit;
        line=1;
            break;
    }
    default:
        break;
    }
}

void pay_func::pay_font_focus(int local)
{
    switch(local)
    {
    case 1://在输手机号
    {
        print=pay_password_edit;
        line =3;
            break;
    }
    case 2://第二次输手机号
    {
        print=pay_telnum_edit;
        line=1;
            break;
    }
    case 3://输入密码
    {
           print=pay_re_telnum_edit;
           line=2;
        break;
    }
    default:
        break;
    }
}


int pay_func::pay_if_pwd_ok(QString pwd,int money)
{
    char **result=new char*;
    char sql[120]={0};
    int col,row;
    dbsingle *db=dbsingle::getinstace();
    sprintf(sql,"select *from card where pwd=%s and state=1 and money_num=%d",(&pwd)->toLatin1().data(),money);
    db->gettable(sql,&result,col,row);
    if(row!=1)
        return 0;
    else
    {
        char sql_update[120]={0};
        sprintf(sql_update,"update card set state=2 where pwd=%s",(&pwd)->toLatin1().data());
        db->gettable(sql_update,&result,col,row);
        memset(sql_update,0,120);
        memset(sql,0,120);
        sprintf(sql,"select balan from balance where tel=\'%s\' ",this->pay_telnum_edit->text().toLatin1().data());
        char **tel=new char*;
        db->gettable(sql,&tel,col,row);
        float sum=QString(tel[1]).toFloat()+money;
        sprintf(sql_update,"update balance set balan=%.2f where tel=\'%s\'",sum,this->pay_telnum_edit->text().toLatin1().data());
        qDebug(sql_update);
        db->gettable(sql_update,NULL,col,row);
        qDebug("s");
        print_record();
        return 1;
    }
}

void pay_func::print_record()
{
    QString *filename=new QString(QString("paydetail")+QString(".txt"));//充值记录
    QFile *file=new QFile(*filename);
    file->open(QIODevice::Append|QIODevice::Text);//追加模式|文本模式（识别回车）
    QString *filedata=new QString();
    filedata->append(QString( "\n时间: "+QDate::currentDate().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")+QString("\n手机号码:")+this->pay_telnum_edit->text()+QString("\n充值密码:")+pay_password_edit->text()+QString("\n金额:")+
                              pay_money_num_combobox->itemText(pay_money_num_combobox->currentIndex())+
                     QString("\n操作员:")+mainfunc->login->get_id()));
    QTextStream *out=new QTextStream(file);
    out->operator <<(*filedata);
    file->close();
}



