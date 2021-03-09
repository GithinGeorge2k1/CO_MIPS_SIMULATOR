#include "Data.h"
#include <QDebug>
#include <QRegExp>
#include <string.h>
#include "Maps.h"
#include <QChar>
#include <cmath>
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
    variableMap.clear();
    labelMap.clear();
}
void debugInstruction(int I)
{
    QString test="";
    for(int i=31;i>=0;i--){
        char c=(char)(((I>>i)&1)+48);
        test.append(QChar(c));
    }
    qDebug()<<test;

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
bool Data::addCode(QString& text){
    Data* D=Data::getInstance();
    int newInstruction=0;
    int instructionTypeTemplate=8;
    Maps* mapInstance=Maps::getInstance();

    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
    int i=0;
    //IF LABEL IS SEPARATELY SEEN ON A LINE IT IS VALID....
    if(list.length()==1){
        if(list.at(0).contains(":") && D->labelMap.contains(list.at(0).section(":",0,0))){
            return true;
        }
        else{
            return false;
        }
    }
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
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+2)]<<(5+5+5+6));
                }
                else
                    return false;
                break;

            case 1:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==4&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&isValue(list.at(i+2)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (16));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+16));
                    newInstruction=newInstruction | convertToInt(list.at(i+2));
                }
                else
                    return false;
                break;

            case 2:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==4&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&(isValue(list.at(i+2))||isValidLabel(list.at(i+2))))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+16));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (16));
                    if(isValue(list.at(i+2)))
                        newInstruction=newInstruction | convertToInt(list.at(i+2));
                    else
                        newInstruction=newInstruction | (Data::labelMap[list.at(i+2)]-D->instructionSize);
                }
                else
                    return false;
                break;

            case 3:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
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

            case 4:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==3)
                {
                    QString base=list.at(i+1).mid(list.at(i+1).indexOf('(')+1, list.at(i+1).indexOf(')')-list.at(i+1).indexOf('(')-1);
                    int offset=convertToInt(list.at(i+1).left(list.at(i+1).indexOf(('('))));
                    if(isRegisterValid(base)&&isRegisterValid(list.at(i))&&offset%4==0)
                    {
                        newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (16));
                        newInstruction=newInstruction | (Maps::Registers[base] <<(16+5));
                        newInstruction=newInstruction | offset >> 2;
                    }
                    else
                        return false;
                }
                else
                    return false;
                break;


            case 5:
                newInstruction = newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==2&&isRegisterValid(list.at(i)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+5+5+6));
                }
                else
                    return false;
                break;


            case 6:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==3&&isRegisterValid(list.at(i))&&(isValidLabel(list.at(i+1)) || isValue(list.at(i+1))))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << 16);
                    newInstruction=newInstruction | convertToInt(list.at(i+1));
                }
                else
                    return false;
                break;

            case 7:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()==4&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&isValue(list.at(i+2)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+5+5+6));
                    newInstruction=newInstruction | convertToInt(list.at(i+1)) << 6;
                }
                else
                    return false;
                break;

        }
        debugInstruction(newInstruction);
        D->instructions[instructionSize]=newInstruction;
        D->instructionSize++;
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

void Data::run(){

    while(PC!=-1){
        int instruction=instructionFetch(PC);
        instructionDecodeRegisterFetch(instruction);
        if(PC>instructionSize){
            //give warning or return bool;
            return;
        }
    }
}

int Data::instructionFetch(int &pc){
    int result=instructions[pc];
    pc++;
    return result;
}

void Data::instructionDecodeRegisterFetch(int instruction){
    int opCode=(instruction>>26) & 0x3f;
    //R-Type
    if(opCode==0x0){
        int funct=instruction & 0x3f;
        int Rs=(instruction>>21) & 0x1f;
        int Rt=(instruction>>16) & 0x1f;
        int Rd=(instruction>>11) & 0x1f;
        int shamt=(instruction>>6) & 0x1f;
        Execute(funct,R[Rs],R[Rt],Rd,shamt);
    }
    //J - Type instruction
    else if(opCode==0x08){
        int target=instruction & 0x3ffffff;
        Execute(opCode,target);
    }
    //I - Type
    else if(opCode==0x20 || opCode==0x30 ||opCode==0x34 ||opCode==0x14 || opCode==0x10
            ||opCode==0x28 || opCode==0x3c ||opCode==0x8c ||opCode==0xac || opCode==0x0c){
        int Rs=(instruction>>21) & 0x1f;
        int Rt=(instruction>>16) & 0x1f;
        int immediate=instruction & 0xffff;
        Execute(opCode,R[Rs],Rt,immediate);

    }

}
void Data::Execute(int funct,int Rs,int Rt,int Rd,int shamt){
    int result=0;
    switch(funct){
    case 0x0: //sll
        result=Rs << shamt;
        break;

    case 0x20:{//add
        result=Rs+Rt;
        break;
    }
    case 0x22:{//sub
        result=Rs-Rt;
        break;
    }
    case 0x2://srl
        result=Rs >> shamt;
        break;

    case 0xa://slt
        //rs<<rt then rd==1
        Rs<Rt ? result=1:result=0;
        break;

    case 0x8://jr
        result=Rs;
        //set PC to Rs?? or in MEM??
        PC=Rs;
        break;

    case 0x24://and
        result=Rs & Rd;
        break;

    case 0x5://or
        result=Rs | Rd;
        break;
    }

}

void Data::Execute(int opCode,int R1,int R2,int immediate){
    //R2 is index of the destination Register
    //R1 is index of source Register
    int result;
    Data* D=Data::getInstance();
    switch(opCode){
    case 0x20:{//addi
        int r1=D->R[R1];
        result=r1+immediate;
        break;
    }
    case 0x30:{//andi
        int r1=D->R[R1];
        result=r1&immediate;
        break;
    }
    case 0x34:{//ori
        int r1=D->R[R1];
        result=R1|immediate;
        break;
    }
    case 0x14:{//bne
        int r1=D->R[R1];
        int r2=D->R[R2];
        if(r1!=r2)
            PC=PC+immediate;
        break;
    }
    case 0x10:{//beq
        int r1=D->R[R1];
        int r2=D->R[R2];
        if(r1==r2)
            PC=PC+immediate;
        break;
    }
    case 0x28:{//slti
        int r1=D->R[R1];
        if(r1<immediate)
            result=1;
        else
            result=0;
        break;
    }
    case 0x3c://lui
        result=immediate<<16;
        //should be stored in R[R2]
        break;

    case 0x8c:{//lw
        //result is Effective Address
        int r1=D->R[R1];
        result=immediate+r1;
        //value in MEM[result] should be stored in R[R2] in MEM
        break;
    }
    case 0xac:{//sw
        int r1=D->R[R1];
        result=immediate+r1;
        //value stored in R[R2] should be wriitten to MEM[result] in WB
        break;
    }
    }
}

void Data::Execute(int opCode, int target) {
    int result=0;
    if(opCode==0x08){
        result=target;
        //set PC to Rs?? or in MEM??
        PC=target;
    }
    if(opCode==0x0c){
        //jal
        //idk is jal a special case?? It is right?
    }

}

