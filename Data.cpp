#include "Data.h"
#include <QDebug>
#include <QRegExp>
#include <string.h>

Data *Data::Object=0;
Data* Data::getObject(){
    if(Object==NULL){
        Object=new Data();
    }
    return Object;
}
//Member initializer list && constructor implementation
Data::Data() : R{}, PC(0), Stack{}, SP(0), data{}, dataSize(0), instructions{}, instructionSize(0)
{

}

void Data::initialize(){
    //combine with mainWindow.cpp eventCall to reset textWidgets...
    memset(R,0,31);
    PC=0;
    memset(Stack,0,2048);
    SP=0;
    memset(data,0,65536);
    dataSize=0;
    memset(instructions,0,4096);
    instructionSize=0;
}

bool Data::addCode(QString text){
    //text.parse;;;;;;;
    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
//    for(int i=0;i<list.length();i++){
//        qDebug()<< list.at(i)<<"\n";
//    }
    return false;


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

