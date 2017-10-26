#include "widget.h"
#include <QThread>
#include <QTimer>
void Widget::log_write(int type,int func)
{
    QString *filename=new QString(QDate::currentDate().toString("yyyyMMdd")+QString(".log"));
    QFile *file=new QFile(*filename);
    file->open(QIODevice::Append|QIODevice::Text);//追加模式|文本模式（识别回车）
    QString *filedata=new QString();
    filedata->append
            ("\n时间: "+QDate::currentDate().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\n"));
    switch(func)
    {
    case Login:
        filedata->append("功能: 登入\n");
        break;
    case Pay:
        filedata->append("功能: 充值缴费\n");
        break;
    case Buy:
         filedata->append("功能: 买电子卡\n");
         break;
    case Detail:
          filedata->append("功能: 详单查询\n");
          break;
    case Bill:
          filedata->append("功能: 账单查询\n");
          break;
    case Setting:
        filedata->append("功能:系统设置\n");
        break;
    case Balance :
        filedata->append("功能:余额查询\n");
        break;
    }
    if(type==Request)
    {
        filedata->append(QString("类型:请求\n"));
    }
    else if(type==Succeed)
    {
        filedata->append(QString("类型:成功\n"));
     }
    else
        filedata->append(QString("类型:失败\n"));
    filedata->append(QString("终端id: xxxxx\n")+QString("工号: "+this->id_edit->text()+"\n")
                     +QString("交易流水:  ")+QDate::currentDate().toString("MMdd")+QTime::currentTime().toString("hhmmss")
                     +this->id_edit->text().toAscii().toHex()+"\n");
    QTextStream *out=new QTextStream(file);
    out->operator <<(*filedata);
    out->operator <<(QString("请求日志:\n"));
    QByteArray hexdata=filedata->toLocal8Bit().toHex();
    QString return_line("\n");
    int lengh=hexdata.length();
    for(int i=0;i<lengh;)
    {
        out->operator <<(hexdata.at(i++));
        out->operator <<(hexdata.at(i++))<<" ";
        if(!(i%32))
            out->operator <<(return_line);
    }
    file->close();
}

void Widget::set_identy()//设置验证码
{

    QString *iden=new QString;
    int i=0;
    static QFont font("楷体",15,20);
    QDateTime time = QDateTime::currentDateTime(); //获取系统现在的时间
    qsrand(time.toTime_t());
    while(1)
    {
        int num=48+qrand()%43;
        if((num>=58&&num<=64))
        {
            continue;
        }
        else
        {
            i=i+1;
            iden->append(num);
        }
        if(i==4)
        {
            this->identy_pic_label->setText(*iden);
            break;
        }
    }
    identy_pic_label->setFont(font);
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->init();
}

void Widget::time_start()
{
    timer=new QTimer(this);
    identy_edit->clear();
    connect(timer,SIGNAL(timeout()),this,SLOT(set_identy()));
    timer->start(60000);
}
void Widget::login_slot()
{
    this->log_write(Request,Login);
    int row=0,col=0;
    char **result=new char*;
    char sql[80];
    if(QString::compare(identy_edit->text(),identy_pic_label->text(),Qt::CaseInsensitive)!=0)//不区分大小写
    {
        this->log_write(Failed,Login);
       QMessageBox *failed_mes=new QMessageBox;
       failed_mes->setText("登入失败，请输入验证码");
       failed_mes->show();
       this->id_edit->clear();
       this->pwd_edit->clear();
       this->identy_edit->clear();
       this->id_edit->setFocus();
       this->set_identy();
       return;
    }
    sprintf(sql,"select *from user where id=\'%s\' and password=\'%s\'",id_edit->text().toLatin1().data(),
            pwd_edit->text().toLatin1().data());
    dbsingle *stu=dbsingle::getinstace(QString("stu.db").toLatin1().data());
   stu->gettable(sql,&result,row,col);
    if(row==3&&col==1)
    {
        QMessageBox *success_mes=new QMessageBox;
        success_mes->setText("登入成功");
        success_mes->show();
        Mainfunc *mainfunc_widget;
        if(strcmp("1",result[5])==0)
        {
            mainfunc_widget=new Mainfunc(this,ADMIN_LOGIN);
        }
        else
        {
           mainfunc_widget=new Mainfunc(this,VISITOR_LOGIN);
        }
        this->log_write(Succeed,Login);//写入日志
        disconnect(timer,SIGNAL(timeout()),this,SLOT(set_identy()));
        timer->stop();
        delete timer;
        this->hide();
        connect(success_mes,SIGNAL(buttonClicked(QAbstractButton*)),mainfunc_widget,SLOT(show()));
    }
    else
    {
         this->log_write(Failed,Login);
        QMessageBox *failed_mes=new QMessageBox;
        failed_mes->setText("登入失败，请输入正确的账号密码");
        failed_mes->show();
        this->id_edit->clear();
        this->pwd_edit->clear();
        this->identy_edit->clear();
        this->id_edit->setFocus();
        this->set_identy();
        this->time_start();
    }
}
void Widget::init()
{
    QIcon icon("./image/icon");
    QPalette background_pal;
    QImage  background_image("./image/back1.jpg");
    this->setWindowIcon(icon);
    background_pal.setBrush(this->backgroundRole(),QBrush(background_image));
    this->setPalette(background_pal);
    QSize background_size(background_image.width(),background_image.height());
    this->setMinimumSize(background_size);
    this->setMaximumSize(background_size);
    this->id_edit=new QLineEdit();
    this->pwd_edit=new QLineEdit();
    this->identy_edit=new QLineEdit();
    pwd_edit->setMaxLength(6);
    pwd_edit->setEchoMode(QLineEdit::Password);
    this->id_label=new QLabel("工号");
    this->pwd_label=new QLabel("密码");
    this->identy_label=new QLabel("验证码");
    this->identy_pic_label=new QLabel("");
    set_identy();
    this->login_pushbutton=new QPushButton("登入");
    this->cancle_pushbutton=new QPushButton("退出");
    QGridLayout *glayout=new QGridLayout();
    glayout->setMargin(30);
    glayout->setVerticalSpacing(20);
    glayout->addWidget(id_label,0,1,1,3);
    glayout->addWidget(id_edit,0,5,1,3);
    glayout->addWidget(pwd_label,1,1,1,3);
    glayout->addWidget(pwd_edit,1,5,1,3);
    glayout->addWidget(identy_label,2,1,1,3);
    glayout->addWidget(identy_edit,2,5,1,3);
    glayout->addWidget(identy_pic_label,2,8,1,2);
    QHBoxLayout *hlayout=new QHBoxLayout();
    hlayout->addWidget(login_pushbutton);
    hlayout->addWidget(cancle_pushbutton);
    hlayout->setMargin(50);
    hlayout->setSpacing(20);
    QVBoxLayout *vlayout=new QVBoxLayout(this);
    vlayout->addLayout(glayout);
    vlayout->addLayout(hlayout);
    this->setLayout(vlayout);
    connect(login_pushbutton,SIGNAL(clicked()),this,SLOT(login_slot()));
    connect(id_edit,SIGNAL(returnPressed()),pwd_edit,SLOT(setFocus()));
    connect(pwd_edit,SIGNAL(returnPressed()),identy_edit,SLOT(setFocus()));
    connect(identy_edit,SIGNAL(returnPressed()),this,SLOT(login_slot()));
    connect(cancle_pushbutton,SIGNAL(clicked()),this,SLOT(close()));
    time_start();//开启计时器
}
Widget::~Widget()
{
    
}
