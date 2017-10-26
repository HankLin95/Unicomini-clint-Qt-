#ifndef BILL_FUNC_H
#define BILL_FUNC_H
#include "func.h"
#include <QDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QStringList>

class bill_func : public func
{
    Q_OBJECT
public:
    bill_func(Mainfunc* mainfunc);
    void init_layout();
    void set_focus();
    QLineEdit *print;
    bool eventFilter(QObject *o, QEvent *e);
private:
    QLabel* bill_tel_num_label;
    QLabel *bill_time_label;
    QLineEdit *bill_edit[2];
    QPushButton* bill_confirm_button;
    QPushButton *bill_cancel_button;
    QDialog *bill_dialog;
    QStringList result_list;
signals:
    
public slots:
    void bill_push_confirm_slot();
    void push_print_slot();//打印
    void press_up_down_slot();
    void press_cancel_slot();
    void bill_append_lineedit_slot();
    void press_left_slot();
    void press_right_slot();
    void press_backspace_slot();
};

#endif // BILL_FUNC_H
