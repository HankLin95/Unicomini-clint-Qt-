#ifndef IPADDRESSWIDGET_H
#define IPADDRESSWIDGET_H

#include <QSpinBox>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QEvent>
#include<QMessageBox>
//IP地址输入框
class IpAddressWidget : public QWidget
{
    Q_OBJECT
private:

    QLabel *m_Label[3];
    QString m_Separator;        //四个数字之间的分隔符
protected:
    bool eventFilter(QObject *object, QEvent *event);
public:
    QSpinBox *m_SpinBox[4];
    void InitIpaddress(QString ip);
    IpAddressWidget(QWidget *parent = 0);
    ~IpAddressWidget();
    void set_focus();
    QString GetIpAddressString();
    void SetSeparator(const QString &separator);
};



#endif // IPADDRESSWIDGET_H
