#ifndef FUNC_H
#define FUNC_H
#include "mainfunc.h"

class func : public QWidget
{
    Q_OBJECT
public:
    explicit func(QWidget *parent = 0);
    virtual void init_layout()=0;
    virtual void set_focus() {}
protected:
    Mainfunc* mainfunc;
signals:
    
public slots:
    
};

#endif // FUNC_H
