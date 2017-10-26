#ifndef DETAIL_FUN_H
#define DETAIL_FUN_H

#include "mainfunc.h"
#include "func.h"
#include <QTableWidget>
#include <QDialog>
#include <QRegExp>
#include<QDate>
#include <QHeaderView>
#include "savetable.h"
class detail_fun : public func
{
    Q_OBJECT
public:
    detail_fun(Mainfunc *mainfunc);
    void init_layout();
    void set_focus();
    QLineEdit* print;
    bool eventFilter(QObject *o, QEvent *e);
    int line;
    bool has_tel();
private:
    Mainfunc* mainfunc;
    /*详单查询控件*/
    QLabel *detail_tel_num_label;
    QLabel *detail_begin_label;
    QLabel *detail_end_label;
    QLineEdit *detail_edit[3];
    QPushButton *detail_confirm_button;
    QPushButton *detail_cancel_button;
    QTableWidget *table;
    QDialog* search_result;
    void detail_move_cursor(int type);
    /**************************************************************************/
signals:
    
public slots:
    void detail_push_confirm_button_slot();
    void detail_append_lineedit_slot();
    void press_direction_button_slot();

    void detail_font_focus();
    void detail_next_focus();
    void press_cancel_slot();
    void order_by(int);
    void print_slot();
};

#endif // DETAIL_FUN_H
