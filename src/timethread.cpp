#include "timethread.h"
#include<QDateTime>
#include "mainfunc.h"
timethread::timethread(QObject *parent) :
    QThread(parent)
{
}

void timethread::run()
{
    while(1)
    {
         Mainfunc::curren_time->setText(QDateTime::currentDateTime().toString("MM-dd hh:mm:ss"));
        QThread::sleep(1);
    }
}
