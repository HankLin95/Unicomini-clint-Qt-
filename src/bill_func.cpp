#include "bill_func.h"
#include <QEvent>
bill_func::bill_func(Mainfunc* mainfunc)
{
    this->mainfunc=mainfunc;
}

void bill_func::init_layout()
{
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx );
    bill_tel_num_label=new QLabel("请输入要查询的手机号码");
    bill_time_label=new QLabel("请输入年月份");
    bill_edit[0]=new QLineEdit;
    bill_edit[0]->setValidator(validator);
    bill_edit[0]->installEventFilter(this);
    bill_edit[0]->setMaxLength(11);
    bill_edit[1]=new QLineEdit;
    bill_edit[1]->setValidator(validator);
    bill_edit[1]->installEventFilter(this);
    bill_confirm_button=new QPushButton("确定");
    bill_cancel_button=new QPushButton("取消");
    QHBoxLayout *bill_hlayout[3]=
    {
        new QHBoxLayout,
        new QHBoxLayout,
        new QHBoxLayout
    };
    bill_hlayout[0]->addWidget(bill_tel_num_label);
    bill_hlayout[0]->addWidget(bill_edit[0]);
    bill_hlayout[1]->addWidget(bill_time_label);
    bill_hlayout[1]->addWidget(bill_edit[1]);
    bill_hlayout[2]->addWidget(bill_confirm_button);
    bill_hlayout[2]->addWidget(bill_cancel_button);
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    for(int i=0;i<3;i++)
    {
        mainfunc->fun_layout->addLayout(bill_hlayout[i]);
    }
    connect(bill_edit[0],SIGNAL(returnPressed()),bill_edit[1],SLOT(setFocus()));
    connect(bill_edit[1],SIGNAL(returnPressed()),this,SLOT(bill_push_confirm_slot()));
     connect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_up_down_slot()));
    connect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_up_down_slot()));
    connect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_left_slot()));
    connect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_right_slot()));
    connect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_backspace_slot()));
    connect(bill_confirm_button,SIGNAL(clicked()),this,SLOT(bill_push_confirm_slot()));
    connect(bill_cancel_button,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    for(int i=0;i<10;i++)
    {
        connect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(bill_append_lineedit_slot()));
    }

}

void bill_func::set_focus()
{
    bill_edit[0]->setFocus();
    print=bill_edit[0];
}
void bill_func::bill_push_confirm_slot()
{
    mainfunc->login->log_write(Request,Bill);
    QDate edit_date=QDate::fromString(this->bill_edit[1]->text(),"yyyyMM");
    if(edit_date.addMonths(6)<QDate::currentDate())
    {
        QMessageBox::warning(0,"错误","只能查询六个月以内的详单记录");
        bill_edit[1]->clear();
         mainfunc->login->log_write(Failed,Bill);
        return ;
    }
    if(edit_date>QDate::currentDate())
    {
        QMessageBox::warning(0,"错误","请输入正确日期");
        bill_edit[1]->clear();
         mainfunc->login->log_write(Failed,Bill);
        return ;
    }
        dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char sql[120]={0};
    sprintf(sql,"select *from bill where tel=\'%s\' and month=\'%s\'",this->bill_edit[0]->text().toLatin1().data()
            ,bill_edit[1]->text().toLatin1().data());
    db->gettable(sql,&result,col,row);
    if(row==0)
    {
        mainfunc->login->log_write(Failed,Bill);
        QMessageBox::warning(this,"错误","不存在该号码该月的账单信息");
        return;
    }
    mainfunc->login->log_write(Succeed,Bill);
    bill_dialog=new QDialog(this);
    QLabel *tel_num_label=new QLabel("客户手机");
    QLabel *time_label=new QLabel("账单时间");
    QLabel *bill_sum_label=new QLabel("账单总额：");
    QLabel *tel_fare_label=new QLabel("    本地通话费");
    QLabel *mes_fare_label=new QLabel("    短信息");
    QLabel *gprs_label=new QLabel("     GPRS上网费");
    QLabel *add_fare_label=new QLabel("     增值业务");
    QLabel *sp_label=new QLabel("    第三方SP业务");
    QLineEdit *tel_num_edit=new QLineEdit("");
    QLineEdit *time_edit=new QLineEdit("");
    QLineEdit *tel_fare_edit=new QLineEdit("");
    QLineEdit *mes_fare_edit=new QLineEdit("");
    QLineEdit *gprs_edit=new QLineEdit(" ");
    QLineEdit *add_fare_edit=new QLineEdit("");
    QLineEdit *sp_edit=new QLineEdit("");
    QPushButton *confrim_button=new QPushButton("    打印凭证");
    QPushButton *cancel_button=new QPushButton("取消");

    tel_num_edit->setText(QString(result[7]));
    time_edit->setText(QString(result[8]));
    tel_fare_edit->setText(QString(result[9]));
    mes_fare_edit->setText(QString(result[10]));
    gprs_edit->setText(QString(result[11]));
    add_fare_edit->setText(QString(result[12]));
    sp_edit->setText(QString(result[13]));

    result_list<<"客户手机"<<tel_num_edit->text().toLatin1().data()<<"账单时间"<<time_edit->text().toLatin1().data()
              <<"本地通话费:"<<tel_fare_edit->text().toLatin1().data()
            <<"短信息"<<mes_fare_edit->text().toLatin1().data()<<"gprs:"
           <<gprs_edit->text().toLatin1().data()<<"增值业务"<<add_fare_edit->text().toLatin1().data()
          <<"第三方业务"<<sp_edit->text().toLatin1().data();
    QHBoxLayout *bill_result_hlayout[9];
    for(int i=0;i<9;i++)
    {
        bill_result_hlayout[i]=new QHBoxLayout;
    }
    bill_result_hlayout[0]->addWidget(tel_num_label);
    bill_result_hlayout[0]->addWidget(tel_num_edit);
    bill_result_hlayout[1]->addWidget(time_label);
    bill_result_hlayout[1]->addWidget(time_edit);
    bill_result_hlayout[2]->addWidget(bill_sum_label);
    bill_result_hlayout[3]->addWidget(tel_fare_label);
    bill_result_hlayout[3]->addWidget(tel_fare_edit);
    bill_result_hlayout[4]->addWidget(mes_fare_label);
    bill_result_hlayout[4]->addWidget(mes_fare_edit);
    bill_result_hlayout[5]->addWidget(gprs_label);
    bill_result_hlayout[5]->addWidget(gprs_edit);
    bill_result_hlayout[6]->addWidget(add_fare_label);
    bill_result_hlayout[6]->addWidget(add_fare_edit);
    bill_result_hlayout[7]->addWidget(sp_label);
    bill_result_hlayout[7]->addWidget(sp_edit);
    bill_result_hlayout[8]->addWidget(confrim_button);
    bill_result_hlayout[8]->addWidget(cancel_button);
    QVBoxLayout *bill_result_vlayout=new QVBoxLayout(bill_dialog);
    for(int i=0;i<9;i++)
    {
        bill_result_vlayout->addLayout(bill_result_hlayout[i]);
    }
    connect(confrim_button,SIGNAL(clicked()),this,SLOT(push_print_slot()));
    connect(cancel_button,SIGNAL(clicked()),bill_dialog,SLOT(close()));
    bill_dialog->exec();
    delete bill_dialog;
}
void bill_func::push_print_slot()
{
    QString *filename=new QString(bill_edit[0]->text()+bill_edit[1]->text()+QString(".txt"));
    QFile *file=new QFile(*filename);
    file->open(QIODevice::WriteOnly|QIODevice::Text);

    QString *filedata=new QString();
    int i=result_list.length();
    qDebug("ss");
    for(int j=0;j<i;j++)
    {

        filedata->append(result_list.at(j));
        qDebug("%s",result_list.at(j).toLatin1().data());
    }
    QTextStream *out=new QTextStream(file);
    out->operator <<(*filedata);
    file->close();
    QMessageBox::information(0,"成功","成功打印凭条");
    bill_dialog->close();
    this->press_cancel_slot();
}
void bill_func::press_up_down_slot()
{
    int i=0;
    if(print==bill_edit[0])
    {
        i=1;
    }
    print=bill_edit[i];
    bill_edit[i]->setFocus();
}
void bill_func::press_cancel_slot()
{
    disconnect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_backspace_slot()));
    disconnect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    disconnect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_up_down_slot()));
   disconnect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_up_down_slot()));
   for(int i=0;i<10;i++)
   {
       disconnect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(bill_append_lineedit_slot()));
   }
   mainfunc->back_mainfunc_slot();
}
void bill_func::bill_append_lineedit_slot()
{
        int corsor_pos;
        corsor_pos=print->cursorPosition();
        print->setText(print->text().insert(corsor_pos,QString(((QPushButton*)(sender()))->text())));
        print->setCursorPosition(corsor_pos+1);
        print->setFocus();
}
bool bill_func::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress)
    {
        print=(QLineEdit*)o;
        print->setCursorPosition(print->text().length());
        return true;
    }
    return QWidget::eventFilter(o,e);
}
void bill_func::press_left_slot()
{
    Mainfunc::move_cursor( print, -1);
}
void bill_func::press_right_slot()
{
    Mainfunc::move_cursor(print,1);
}

void bill_func::press_backspace_slot()
{
    Mainfunc::backspace(print);
}




