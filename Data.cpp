#include "Data.h"
#include <QDebug>
#include <QRegExp>

Data::Data() : R{}, PC(0), Stack{}, data{}, Text{}{}

void Data::addCode(QString text){
    //text.parse;;;;;;;
    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
//    for(int i=0;i<list.length();i++){
//        qDebug()<< list.at(i)<<"\n";
//    }



}

QString Data::displayRegisters(){
    QString text="";
    text.append(QString("PC      = %1\n").arg(PC));
    for(int i=0;i<31;i++)
    {
        text.append(QString("R[%1]      = %2\n").arg(i).arg(R[i]));
    }
    return text;

}

