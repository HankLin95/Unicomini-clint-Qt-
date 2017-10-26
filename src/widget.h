#ifndef WIDGET_H
#define WIDGET_H
#define Request 1
#define Succeed 2
#define Failed 10
#define Login 3
#define Pay 4
#define Buy 5
#define Bill 6
#define Detail 7
#define Setting 8
#define Balance 9
#include <QWidget>
#include <QIcon>
#include <QTextCodec>
#include <QPalette>
#include <QSize>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "dbsingleton.h"
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QtNetwork/QUdpSocket>
#include "mainfunc.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void idpwd_clear()
    {
        id_edit->clear();
        pwd_edit->clear();
        identy_edit->clear();
        set_identy();
        id_edit->setFocus();
    }
    void time_start();
    QString get_id()
    {
        return id_edit->text();
    }

    void init();
    ~Widget();
    void  log_write(int type,int func);
private:

    QLineEdit *id_edit;
    QLineEdit *pwd_edit;
    QLineEdit *identy_edit;
    QLabel *id_label;
    QLabel *pwd_label;
    QLabel *identy_label;
    QLabel *identy_pic_label;
    QPushButton *login_pushbutton;
    QPushButton *cancle_pushbutton;
    QTimer *timer;
    QUdpSocket*sender;
private slots:
    void login_slot();
    void set_identy();
};


#endif // WIDGET_H
