#include "Data.h"
#include <QDebug>
#include <QRegExp>
#include <string.h>


//Find yy this works(TutorialsPoint - singleton class)
Data *Data::instance=0;
//=====================================================//


Data* Data::getInstance(){
    if(instance==NULL){
        instance=new Data;
    }
    return instance;
}


//Member initializer list && constructor implementation
Data::Data() : R{}, PC(0), Stack{}, SP(0), data{}, dataSize(0), instructions{}, instructionSize(0)
{

}

void Data::initialize(){
    //combine with mainWindow.cpp eventCall to reset textWidgets...
    memset(R,0,sizeof (R));
    PC=0;
    memset(Stack,0,sizeof (Stack));
    SP=0;
    memset(data,0,sizeof (data));
    dataSize=0;
    memset(instructions,0,sizeof (instructions));
    instructionSize=0;
}

bool Data::addCode(QString& text){
    //text.parse;;;;;;;
    int newInstruction=0;
    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
//    for(int i=0;i<list.length();i++){
//        qDebug()<< list.at(i)<<"\n";
//    }
    int i=0;

    //checking for a label...
    if(i<list.length() && list.at(i).indexOf(":")==list.at(i).length()-1){
        labelMap[list.at(i).section(':',0,0)]=instructionSize;
        i++;
    }

    if(i<list.length()){
        //we have to create map somewhere
        if(reg.contains(list.at(i))){
            newInstruction=newInstruction | map[list.at(i)];
            i++;
        }
        else{
            return false;
        }
    }
    if(i<list.length()){
        //we have to create map somewhere
        if(map.contains(list.at(i))){
            newInstruction=newInstruction | map[list.at(i)];
            i++;
        }
        else{
            return false;
        }
    }

    if(i<list.length()){
        //we have to create map somewhere
        if(map.contains(list.at(i))){
            newInstruction=newInstruction | map[list.at(i)];
            i++;
        }
        else{
            return false;
        }
    }

    if(i<list.length()){
        //we have to create map somewhere
        if(map.contains(list.at(i))){
            newInstruction=newInstruction | map[list.at(i)];
            i++;
        }
        else{
            return false;
        }
    }

    //cant use switch case... then use what??
    //think a separate qmap is the best way to go(like qmap for functions and corresponding values)...in case of registers we can do bit shifting...


//    instructionSize++;
    return true;
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

