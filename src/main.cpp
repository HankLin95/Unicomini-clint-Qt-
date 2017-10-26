#include "widget.h"
#include <QApplication>
#include <QFont>
#include <QDate>
#include "timethread.h"

int main(int argc, char *argv[])
{
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font("宋体",9,QFont::SmallCaps);

  QApplication a(argc, argv);
    a.setFont(font);

    Widget w;
     w.show();


    return a.exec();
}
