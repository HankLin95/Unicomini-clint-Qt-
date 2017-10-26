#ifndef SAVETABLE_H
#define SAVETABLE_H

#include <QObject>
#include<QTableWidget>

class savetable : public QObject
{
    Q_OBJECT
public:
    explicit savetable(QObject *parent = 0);
    bool save_table(QTableWidget*table);
signals:
    
public slots:
    
};

#endif // SAVETABLE_H
