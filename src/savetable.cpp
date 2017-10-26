#include "savetable.h"
#include<QTextEdit>
#include<QTextStream>
#include<QFileDialog>
savetable::savetable(QObject *parent) :
    QObject(parent)
{
}

bool savetable::save_table(QTableWidget *table)
{
    QString filepath = QFileDialog::getSaveFileName(0,QString("save"),QString("./detail"),QString("EXCEL files (*.xls *.xlsx);;TEXT files(*txt);;all files(*)"));
    if(filepath.isEmpty())
    {
        return 0;
    }
    int row = table->rowCount();
    int col = table->columnCount();
    QList<QString> list;
    QString HeaderRow;
    for(int i=0;i<col;i++)
    {
        HeaderRow.append(table->horizontalHeaderItem(i)->text()+"\t");
    }
    list.push_back(HeaderRow);
    for(int i=0;i<row;i++)
    {
        QString rowStr = "";
        for(int j=0;j<6;j++){
            if(table->item(i,j)->text().isEmpty())
            {
                continue;
            }
            rowStr += table->item(i,j)->text() + "\t";
        }
        list.push_back(rowStr);
    }
    QTextEdit textEdit;
    for(int i=0;i<list.size();i++)
    {
        textEdit.append(list.at(i));
    }
    QFile file(filepath);
    if(file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts<<textEdit.document()->toPlainText();
        file.close();
    }
    return 1;
}
