#-------------------------------------------------
#
# Project created by QtCreator 2016-03-27T16:50:11
#
#-------------------------------------------------

QT       += core gui
QT +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Unicomini
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dbsingleton.cpp \
    mainfunc.cpp \
    pay_func.cpp \
    buy_func.cpp \
    balance_fun.cpp \
    detail_fun.cpp \
    bill_func.cpp \
    func.cpp \
    timethread.cpp \
    savetable.cpp \
    setting_fun.cpp

HEADERS  += widget.h \
    sqlite3.h \
    dbsingleton.h \
    mainfunc.h \
    pay_func.h \
    buy_func.h \
    balance_fun.h \
    setting_fun.h \
    detail_fun.h \
    bill_func.h \
    func.h \
    timethread.h \
    savetable.h
LIBS += -LC:\Qt\qtgui\Unicomini -lsqlite3
