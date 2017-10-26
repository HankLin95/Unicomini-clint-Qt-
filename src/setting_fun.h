#ifndef SETTING_FUN_H
#define SETTING_FUN_H

#include <QWidget>
#include "mainfunc.h"
#include "func.h"
class Mainfunc;
class setting_fun : public func
{
    Q_OBJECT
public:
    setting_fun(Mainfunc *mainfun);
    void init_layout();
    bool eventFilter(QObject *o, QEvent *e);
    void set_focus();
    QLineEdit *print;
void setting_next_focus( );
void setting_font_focus( );
private:
    int line;
    Mainfunc *mainfunc;
    QLabel *setting_ip_label;
    QLabel *setting_port_label;
    QLabel *setting_device_id_label;
    QLabel *setting_soft_version_label;
    QLabel *setting_hard_version_label;
    QLineEdit *setting_edit[5];
    QPushButton *setting_confirm_button;
    QPushButton *setting_cancel_button;

    char *pre_ip;
    void add_dbdata();
signals:
    
public slots:
    void setting_append_lineedit_slot();
    void press_direction_button_slot();
    void press_confirm_button_slot();
    void press_cancel_slot();
};

#endif // SETTING_FUN_H
