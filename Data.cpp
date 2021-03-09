#include "Data.h"
#include <QDebug>
#include <QRegExp>
#include <string.h>
#include "Maps.h"
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
void debugInstruction(int I)
{
    for(int i=31;i>=0;i--)
       qDebug()<<((I>>i)&1);
    qDebug()<<"\n";

}
bool isRegisterValid(QString R)
{
    return Maps::Registers.contains(R);
}
bool isValue(QString R)
{

    if(R.contains("0x")){
        R=R.section("0x",1,1);
        bool bstatus=false;
        R.toInt(&bstatus,16);
        return bstatus;
    }
    else
    {
        QRegExp exp("\\d*");
        return exp.exactMatch(R);
    }
}
int convertToInt(QString R)
{
    if(R.contains("0x"))
    {
        R=R.section("0x", 1, 1);
        bool bstatus=false;
        return R.toInt(&bstatus,16);
    }
    else
        return R.toInt();
}
bool isValidLabel(QString L)
{
    Data* D=Data::getInstance();
    return (D->labelMap.contains(L));
}
bool Data::addCode(QString& text, int currentLineNo){
    int newInstruction=0;
    int instructionTypeTemplate=8;
    Maps* mapInstance=Maps::getInstance();

    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
    int i=0;

    //checking for a label...
    if(i<list.length() && list.at(i).indexOf(":")==list.at(i).length()-1){
        labelMap[list.at(i).section(':',0,0)]=instructionSize;
        i++;
    }


    //Do From Here......
    if(Maps::Commands.contains(list.at(i)))
    {
        instructionTypeTemplate=Maps::Commands[list.at(i)].second;
        switch(instructionTypeTemplate)
        {
            case 0:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==4  &&  isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&isRegisterValid(list.at(i+2)))
                {
                    newInstruction=newInstruction | Maps::Registers[list.at(i)] << (5+6);
                    newInstruction=newInstruction | Maps::Registers[list.at(i+1)] << (5+5+6);
                    newInstruction=newInstruction | Maps::Registers[list.at(i+2)]<<(5+5+5+6);
                }
                else
                    return false;
                break;

            case 1:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first << (5+5+16);
                i++;
                if(list.length()==4&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&isValue(list.at(i+2)))
                {
                    newInstruction=newInstruction | Maps::Registers[list.at(i)] << (5+16);
                    newInstruction=newInstruction | Maps::Registers[list.at(i+1)] << (5+5+16);
                    newInstruction=newInstruction | convertToInt(list.at(i+2));
                }
                else
                    return false;
                break;

            case 4:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first << (5+5+16);
                i++;
                if(list.length()==3)
                {
                    QString base=list.at(i+1).mid(list.at(i+1).indexOf('('), list.at(i+1).indexOf(')'));
                    int offset=convertToInt(list.at(i+1).left(list.at(i+1).indexOf(('('))));
                    if(isRegisterValid(base)&&isRegisterValid(list.at(i))&&offset%4==0)
                    {
                        newInstruction=newInstruction | Maps::Registers[list.at(i)] << (16);
                        newInstruction=newInstruction | Maps::Registers[base] <<(16+5);
                        newInstruction=newInstruction | offset >> 2;
                    }
                    else
                        return false;
                }
                else
                    return false;
                break;


            case 2:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first << (5+5+16);
                i++;
                if(list.length()==4&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&(isValue(list.at(i+2))||isValidLabel(list.at(i+2))))
                {
                    newInstruction=newInstruction | Maps::Registers[list.at(i)] << (5+5+16);
                    newInstruction=newInstruction | Maps::Registers[list.at(i+1)] << (5+16);
                    if(isValue(list.at(i+2)))
                        newInstruction=newInstruction | convertToInt(list.at(i+2));
                    else
                        newInstruction=newInstruction | Data::labelMap[list.at(i+2)]-currentLineNo;
                }
                else
                    return false;
                break;

            case 5:
                newInstruction = newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==2&&isRegisterValid(list.at(i)))
                {
                    newInstruction=newInstruction | Maps::Registers[list.at(i)] << (5+5+5+6);
                }
                else
                    return false;
                break;

            case 3:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first << 26;
                i++;
                if(list.length()==2)
                {
                    if(isValidLabel(list.at(i)))
                        newInstruction=newInstruction | Data::labelMap[list.at(i)];

                    else if(isValue(list.at(i)))
                        newInstruction=newInstruction | convertToInt(list.at(i));
                    else
                        return false;
                }
                else
                    return false;
                break;

            case 6:
                newInstruction=newInstruction | newInstruction | Maps::Commands[list.at(i)].first << (5+5+16);
                i++;
                if(list.length()==3&&isRegisterValid(list.at(i))&&isValidLabel(list.at(i+1)))
                {
                    newInstruction=newInstruction | Maps::Registers[list.at(i)] << 16;
                    newInstruction=newInstruction | convertToInt(list.at(i+1));
                }
                else
                    return false;
            break;
        }
        debugInstruction(newInstruction);
        instructionSize++;
        return true;
    }
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

QString Data::displayData(){
    Data *x=Data::getInstance();
    QString text="";
    int DummyAddress=2000;
    for(int i=0;i<x->dataSize;i++){
        text.append(QString("[%1]   %2   ").arg(DummyAddress).arg(x->data[i]));
        DummyAddress+=4;
    }
    return text;
}

