#include "buy_func.h"
#include <QTextEdit>
buy_func::buy_func( Mainfunc *mainfunc)
{
    this->mainfunc=mainfunc;
}

void buy_func::init_layout()
{
    buy_money_num_label=new QLabel("选择购买金额");//买卡金额
    buy_card_num_choose_label=new QLabel("购买张数");//数量
    QStringList card_money=(QStringList()<<"30"<<"50"<<"100");
    QStringList card_num;
    for(int i=1;i<=20;i++)
    {
        card_num.append(QString::number(i,10));
    }
    buy_money_num_combobox=new QComboBox;//金额
    buy_money_num_combobox->addItems(card_money);
    buy_card_num_choose_combobox=new QComboBox;//数量
    buy_card_num_choose_combobox->addItems(card_num);
    buy_confirm_pushbutton=new QPushButton("确定");//确定购买
    buy_cancel_buy_pushbutton=new QPushButton("取消");//取消购买
    QHBoxLayout *buy_hlayout[3]={
        new QHBoxLayout,new QHBoxLayout,new QHBoxLayout
    };
    buy_hlayout[0]->addWidget(buy_money_num_label);
    buy_hlayout[0]->addWidget(buy_money_num_combobox);
    buy_hlayout[1]->addWidget(buy_card_num_choose_label);
    buy_hlayout[1]->addWidget(buy_card_num_choose_combobox);
    buy_hlayout[2]->addWidget(buy_confirm_pushbutton);
    buy_hlayout[2]->addWidget(buy_cancel_buy_pushbutton);
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    mainfunc->fun_layout->addLayout(buy_hlayout[0]);
    mainfunc->fun_layout->addLayout(buy_hlayout[1]);
    mainfunc->fun_layout->addLayout(buy_hlayout[2]);
    connect(buy_confirm_pushbutton,SIGNAL(clicked()),this,SLOT(push_confirm_slot()));
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    connect(buy_cancel_buy_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
}
void buy_func::press_cancel_slot()
{
    disconnect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    mainfunc->back_mainfunc_slot();
}
void buy_func::push_confirm_slot()
{
    int money_num,card_num;
    money_num=buy_money_num_combobox->itemText(buy_money_num_combobox->currentIndex()).toInt();
    card_num=buy_card_num_choose_combobox->itemText(buy_card_num_choose_combobox->currentIndex()).toInt();
    QMessageBox *if_buy=new QMessageBox();
    if_buy->setWindowTitle("买电子卡");
    if_buy->setText("你选择购买"+QString::number(card_num)+"张"
                    +"面额为"+QString::number(money_num)+"元的电子卡，是否确定购买？");
    if(buy_has_card(money_num,card_num))//此处添加判断是否有这么多张
    {
        QPushButton *if_buy_ok_btn=if_buy->addButton("确定",QMessageBox::AcceptRole);
        QPushButton *if_buy_no_btn=if_buy->addButton("取消",QMessageBox::RejectRole);
        if_buy->exec();
        if(if_buy->clickedButton()==if_buy_ok_btn)//选择确定购买这些电子卡
        {
               mainfunc->login->log_write(Succeed,Buy);
            buy_print_card(money_num,card_num);
        }
        else if(if_buy->clickedButton()==if_buy_no_btn)
        {
             mainfunc->login->log_write(Failed,Buy);
        }
        else
        {}
        delete if_buy_ok_btn;
        delete if_buy_no_btn;
    }
    else
    {
        QString id=this->mainfunc->login->get_id();
        qDebug(id.toLatin1().data());
        char**result=new char*;
        int col,row;
        char sql[100]={0};
        sprintf(sql,"select role from user where id=\'%s\'",id.toLatin1().data());
        dbsingle*db=dbsingle::getinstace();
        db->gettable(sql,&result,col,row);
        qDebug(result[1]);
        if(strcmp(result[1],"1")==0)
        {
            //管理员
            QMessageBox* if_add_card=new QMessageBox(this);
            if_add_card->setText( QString("没有足够的电子卡了，是否增加电子卡"));
            QPushButton *ok=if_add_card->addButton(QString("是"),QMessageBox::AcceptRole);
            QPushButton*no=if_add_card->addButton(QString("否"),QMessageBox::RejectRole);
            if_add_card->exec();
            if(if_add_card->clickedButton()==ok)
            {
                int add_num=buy_add_card(money_num,card_num);
                QMessageBox::information(this,"成功",QString("成功添加"+QString::number(add_num)+"张"+QString::number(money_num)+"元电子卡"));
            }
            else
            {
                delete if_add_card;
            }
        }
        else
        {
            QMessageBox::warning(this,"票务中心","没有足够的电子卡了");
        }
    }
    delete if_buy;
}
int buy_func::buy_add_card( int money_num,int card_num)
{
    int col,row,id;
    char sql[100]={0};
    sprintf(sql,"select id from card");
    dbsingle*db=dbsingle::getinstace();
    db->gettable(sql,NULL,col,row);
    id=row;
    //id就是目前最大卡号
    memset(sql,0,100);
    sprintf(sql,"select * from card where state=0 and money_num=%d",money_num);
    db->gettable(sql,NULL,col,row);
    int add_num=card_num-row;//相差数就是需要的数量
    QDateTime time = QDateTime::currentDateTime(); //获取系统现在的时间
    qsrand(time.toTime_t());
    for(int i=0;i<add_num;i++)
    {
        char *pwd=new char[7];
        memset(pwd,0,7);
        for(int j=0;j<6;j++)
        {
         int num=qrand()%10;
         pwd[j]=num+48;
        }//得到了密码
        qDebug(pwd);
        memset(sql,0,100);
        sprintf(sql,"insert into card values(%d,%d,0,\'%s\')",id++,money_num,pwd);
        if(db->gettable_return(sql,NULL,col,row)==0)
        {
            id--;
            i--;
        }
        qDebug();

    }
    return add_num;
}

bool buy_func::buy_has_card(int money_num,int card_num)
{
    char sql[100];
    char**result=new char*;
    int col,row;
    sprintf(sql,"select *from card where money_num=%d and state=0",money_num);
    dbsingle* db=dbsingle::getinstace();
    db->gettable(sql,&result,col,row);
    if(row>=card_num)
        return true;
    else
        return false;
}
void buy_func::push_print_slot()
{
    QString *file_data=new QString;
    file_data->append(buy_money_num_combobox->itemText(buy_money_num_combobox->currentIndex())
                      +"元"+buy_card_num_choose_combobox->itemText(buy_card_num_choose_combobox->currentIndex())
                      +"张:\n"+*pwd);
    QFile *file=new QFile(QString("./card/")+QDate::currentDate().toString("yyyyMMdd")+mainfunc->login->get_id()+QString(".txt"));
    file->open(QIODevice::Append|QIODevice::Text);
    QTextStream *out=new QTextStream(file);
    out->operator <<(*file_data);
    file->close();
    QMessageBox::information(this,"成功","打印凭条成功");
    buy_succeed_dia->close();
}
void buy_func::buy_print_card(int money_num,int card_num)
{
    dbsingle* db=dbsingle::getinstace();
    pwd=new QString();
    //此函数处理 将找到的行中的 state列改为1  及已卖出
    for(int i=0;i<card_num;i++)
    {
        char sql[100],sql_pwd[80];
        char**result=new char*;
        char **result_pwd;
        int col,row;
        sprintf(sql_pwd,"select pwd from card where state=0 and money_num=%d limit 1 offset 0",money_num);
        db->gettable(sql_pwd,&result_pwd,col,row);
        sprintf(sql,"update card set state=1 where id=(select id from card where state=0 and money_num=%d)",money_num);
        db->gettable(sql,&result,col,row);
        pwd->append(result_pwd[1]);
        pwd->append("/");
    }
    buy_succeed_dia=new QDialog();//以下做将密码显示到电脑上 并可以写入文件
    QPushButton *print_button=new QPushButton("打印凭证");
    QPushButton *cancel_button=new QPushButton("取消");
    QTextEdit *pwd_edit=new QTextEdit(*pwd);
    QLineEdit *card_num_edit=new QLineEdit(QString::number(card_num));
    QLabel *pwd_label=new QLabel("交易密码");
    QLabel *card_num_label=new QLabel("购买张数");
    QGridLayout *pwd_glayout=new  QGridLayout(buy_succeed_dia);
    QHBoxLayout *pwd_hlayout=new QHBoxLayout();
    pwd_glayout->addWidget(pwd_label,2,0,1,2);
    pwd_glayout->addWidget(pwd_edit,0,2,4,5);
    pwd_glayout->addWidget(card_num_label,5,0,1,2);
    pwd_glayout->addWidget(card_num_edit,5,2,1,5);
    pwd_hlayout->addWidget(print_button);
    pwd_hlayout->addWidget(cancel_button);
    pwd_hlayout->setMargin(20);
    pwd_hlayout->setSpacing(35);
    pwd_glayout->addLayout(pwd_hlayout,6,0,1,8);
    connect(print_button,SIGNAL(clicked()),this,SLOT(push_print_slot()));
    connect(cancel_button,SIGNAL(clicked()),buy_succeed_dia,SLOT(close()));
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    buy_succeed_dia->exec();
}




















