#include "detail_fun.h"
#include <QEvent>
#include<QTextEdit>
#include "savetable.h"
#include <QAbstractItemView>
class QFileDialog;
detail_fun::detail_fun(Mainfunc *mainfunc)
{
    this->mainfunc=mainfunc;

}
void detail_fun::init_layout()
{
    line=1;
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx );
    detail_tel_num_label=new QLabel("请输入要查询的手机号码");
    detail_begin_label=new QLabel("请输入起始年月");
    detail_end_label=new QLabel("请输入终止年月");
    for(int i=0;i<3;i++)
    {
        detail_edit[i]=new QLineEdit;
        detail_edit[i]->installEventFilter(this);
        detail_edit[i]->setValidator(validator);
    }
    detail_edit[0]->setMaxLength(11);
    detail_edit[1]->setMaxLength(6);
    detail_edit[2]->setMaxLength(6);
    connect(detail_edit[0],SIGNAL(returnPressed()),detail_edit[1],SLOT(setFocus()));
    connect(detail_edit[1],SIGNAL(returnPressed()),detail_edit[2],SLOT(setFocus()));
    detail_confirm_button=new QPushButton("确定");
    detail_cancel_button=new QPushButton("取消");
    mainfunc->fun_layout=new QVBoxLayout(mainfunc);
    mainfunc->fun_layout->addWidget(Mainfunc::curren_time);
    QHBoxLayout *detail_hlayout[4]=
    {
        new QHBoxLayout,
        new QHBoxLayout,
        new QHBoxLayout,
        new QHBoxLayout
    };
    detail_hlayout[0]->addWidget(detail_tel_num_label);
    detail_hlayout[0]->addWidget(detail_edit[0]);
    detail_hlayout[1]->addWidget(detail_begin_label);
    detail_hlayout[1]->addWidget(detail_edit[1]);
    detail_hlayout[2]->addWidget(detail_end_label);
    detail_hlayout[2]->addWidget(detail_edit[2]);
    detail_hlayout[3]->addWidget(detail_confirm_button);
    detail_hlayout[3]->addWidget(detail_cancel_button);

    for(int i=0;i<4;i++)
    {
        mainfunc->fun_layout->addLayout(detail_hlayout[i]);
    }
    for(int i=0;i<10;i++)
    {
        connect(mainfunc->number_pushbutton[i],SIGNAL(clicked()),this,SLOT(detail_append_lineedit_slot()));
    }
    connect(detail_cancel_button,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    connect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    connect(detail_confirm_button,SIGNAL(clicked()),this,SLOT(detail_push_confirm_button_slot()));
    connect(detail_edit[2],SIGNAL(returnPressed()),this,SLOT(detail_push_confirm_button_slot()));
    connect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    connect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
}

void detail_fun::set_focus()
{
    detail_edit[0]->setFocus();
    print=detail_edit[0];
}
void detail_fun::press_direction_button_slot()
{
    qDebug("aaaa");
    QString direc(((QPushButton*)sender())->text());//获取按键信息
    if(QString::compare(direc,"↑")==0)//判断按键
    {
          detail_font_focus();
    }
    else if(QString::compare(direc,"↓")==0)
    {
        detail_next_focus();
    }
    else if(QString::compare(direc,"←")==0)
    {
        Mainfunc::move_cursor(print,-1);
    }
    else if(QString::compare(direc,"→")==0)
    {
           Mainfunc::move_cursor(print,1);
    }
    else
    {
        Mainfunc::backspace(print);
    }
    print->setFocus();
}
bool detail_fun::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress)
    {
        qDebug("获取到鼠标点击信号");
        for(int i=0;i<3;i++)
        {
            if(o==detail_edit[i])
            {
                line=i+1;
                print=(QLineEdit*)o;
                print->setCursorPosition(print->text().length());
                print->setFocus();
                return true;
            }
        }
    }
    return QWidget::eventFilter(o,e);
}

bool detail_fun::has_tel()
{
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col=0,row=0;
    char sql[150]={0};
    sprintf(sql,"select * from detail where  ( calling=\'%s\' or called=\'%s\') ",detail_edit[0]->text().toLatin1().data(),detail_edit[0]->text().toLatin1().data());
    db->gettable(sql,&result,col,row);
    qDebug("%d,%d",col,row);
    if(row==0)
    {
        return false;
    }
    return true;
}
void detail_fun::detail_next_focus()
{
    if(line<=2)
    {
        line++;
    }
    else
        line=1;
    print=detail_edit[line-1];
}

void detail_fun::press_cancel_slot()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qDebug("utf-8");
    disconnect(mainfunc->up_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->down_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->left_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->right_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->backspace_pushbutton,SIGNAL(clicked()),this,SLOT(press_direction_button_slot()));
    disconnect(mainfunc->back_pushbutton,SIGNAL(clicked()),this, SLOT(press_cancel_slot()));
    mainfunc->back_mainfunc_slot();
}
void detail_fun::order_by(int colum)
{
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char *sql_test=new char[230];
    char sql_order[16]={0};
    switch (colum) {
    case 0:
        strcpy(sql_order,"calling");
        break;
    case 1:
        strcpy(sql_order,"called");
        break;
    case 2:
        strcpy(sql_order,"type");
        break;
    case 3:
        strcpy(sql_order,"SUM(time)");
        break;
    case 4:
        strcpy(sql_order,"SUM(money)");
        break;
    default:
        break;
    }
    if(strlen(sql_order))
    {
        sprintf(sql_test,"select calling,called,type,SUM(time),SUM(money),ps from detail where  ( calling=\'%s\' or called=\'%s\') and month >=%s and month<=%s group by calling,type,called order by %s",
              detail_edit[0]->text().toLatin1().data(),detail_edit[0]->text().toLatin1().data(),  detail_edit[1]->text().toLatin1().data(),detail_edit[2]->text().toLatin1().data(),sql_order);
        db->gettable(sql_test,&result,col,row);
        printf("%s",sql_test);
        for(int i=1;i<=row;i++)
        {
            for(int j=0;j<col;j++)
            {
                table->setItem(i-1,j,new  QTableWidgetItem(result[j+i*col]));
                qDebug(result[j+i*col]);
            }
        }
    }
}
void detail_fun::print_slot()
{
    savetable*save=new savetable();
    if(save->save_table(table))
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QMessageBox::information(0,"打印凭条","凭条已打印");
    search_result->close();
    this->press_cancel_slot();
    }
    delete save;
}

void detail_fun::detail_font_focus()
{
    if(line>=2)
    {
        line--;
    }
    else
        line=3;
       print=detail_edit[line-1];
}
void detail_fun::detail_push_confirm_button_slot()
{
     mainfunc->login->log_write(Request,Detail);
    if(detail_edit[1]->text().length()!=6||detail_edit[2]->text().length()!=6)//此处判断是否填入正确的时间
    {
        QMessageBox::warning(0,"错误","请输入时间(格式为：YYYYMM)");
         mainfunc->login->log_write(Failed,Detail);
        return ;
    }
    if(QString(detail_edit[2]->text()).toInt()-QString(detail_edit[1]->text()).toInt()<=0)
    {
        QMessageBox::warning(0,"错误","起始时间必须小于终止时间");
        detail_edit[1]->clear();
        detail_edit[2]->clear();
         mainfunc->login->log_write(Failed,Detail);
        return ;
    }
    QDate curren=QDate::currentDate();
    QDate begin=QDate::fromString(QString(detail_edit[1]->text()),"yyyyMM");
    if(begin.addMonths(6)<curren)
    {
        QMessageBox::warning(0,"错误","只能查询六个月以内的详单记录");
        detail_edit[1]->clear();
        detail_edit[2]->clear();
         mainfunc->login->log_write(Failed,Detail);
        return ;
    }
    QDate end=QDate::fromString(QString(detail_edit[2]->text()),"yyyyMM");
    if(end>curren)
    {
        QMessageBox::warning(0,"错误","查询日期超出范围");
        detail_edit[1]->clear();
        detail_edit[2]->clear();
         mainfunc->login->log_write(Failed,Detail);
        return ;
    }
    if(!has_tel())
    {
        QMessageBox::warning(0,"错误","没有该号码的详单记录");
         mainfunc->login->log_write(Failed,Detail);
        detail_edit[0]->setFocus();
        return ;
    }
     mainfunc->login->log_write(Succeed,Detail);
    dbsingle*db=dbsingle::getinstace();
    char **result=new char*;
    int col,row;
    char sql[170]={0};
    sprintf(sql,"select calling,called,type,SUM(time),SUM(money),ps from detail where  ( calling=\'%s\' or called=\'%s\') and month >=%s and month<=%s group by calling,type,called",
          detail_edit[0]->text().toLatin1().data(),detail_edit[0]->text().toLatin1().data(),  detail_edit[1]->text().toLatin1().data(),detail_edit[2]->text().toLatin1().data());
    db->gettable(sql,&result,col,row);
    search_result=new QDialog(this);
    QLabel *tel_label=new QLabel("客户手机");
    QLabel *time_label=new QLabel("账单时间");
    QLineEdit *tel_edit=new QLineEdit;
    tel_edit->setText(detail_edit[0]->text());
    QLineEdit *time_edit=new QLineEdit(QString(detail_edit[1]->text()+"~"+detail_edit[2]->text()));
    QPushButton *print_pushbutton=new QPushButton("打印凭证");
    QPushButton *cancel_pushbutton=new QPushButton("取消");
    table=new QTableWidget(row,6);
    QStringList header;
    header<<"主叫号码"<<"被叫号码"<<"通话类型"<<"通话时长"<<"金额 "<<"备注";
    table->setHorizontalHeaderLabels(header);
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    for(int i=1;i<=row;i++)
    {
        for(int j=0;j<col;j++)
        {
            table->setItem(i-1,j,new  QTableWidgetItem(result[j+i*col]));
        }
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout *detail_vlayout=new QVBoxLayout(search_result);
    QHBoxLayout *detail_hlayout[4]=
    {
        new QHBoxLayout,
        new QHBoxLayout,
        new QHBoxLayout,
        new QHBoxLayout
    };
    detail_hlayout[0]->addWidget(tel_label);
    detail_hlayout[0]->addWidget(tel_edit);
    detail_hlayout[1]->addWidget(time_label);
    detail_hlayout[1]->addWidget(time_edit);
    detail_hlayout[2]->addWidget(table);
    detail_hlayout[3]->addWidget(print_pushbutton);
    detail_hlayout[3]->addWidget(cancel_pushbutton);
    for(int i=0;i<4;i++)
    {
        detail_vlayout->addLayout(detail_hlayout[i]);
    }
    search_result->setMaximumWidth(table->width());
    search_result->setMinimumWidth(table->width());//控制dialog宽度，以表格宽度为准
    connect(table->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(order_by(int)));
    connect(print_pushbutton,SIGNAL(clicked()),this,SLOT(print_slot()));
    connect(cancel_pushbutton,SIGNAL(clicked()),search_result,SLOT(close()));
    connect(cancel_pushbutton,SIGNAL(clicked()),this,SLOT(press_cancel_slot()));
    search_result->exec();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}
void detail_fun::detail_append_lineedit_slot()
{
    int corsor_pos;
    corsor_pos=print->cursorPosition();
    print->setText(print->text().insert(corsor_pos,QString(((QPushButton*)(sender()))->text())));
    print->setCursorPosition(corsor_pos+1);
    print->setFocus();
}
