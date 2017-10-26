#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>

class timethread : public QThread
{
    Q_OBJECT
public:
    explicit timethread(QObject *parent = 0);
    void run();
signals:
    
public slots:
    
};

#endif // TIMETHREAD_H
