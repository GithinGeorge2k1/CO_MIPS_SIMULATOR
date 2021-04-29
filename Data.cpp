#include "Data.h"
#include "utilities.h"
#include "Maps.h"
#include "mainwindow.h"

#include <QDebug>
#include <QRegExp>
#include <string.h>
#include <QChar>
#include <cmath>
#include <QMessageBox>

Data* Data::instance=0;
int prevClockCycle;
int prevSpace;
int registerWriteBackLine=-1;
int rindex=0;
int cindexPivot=0;
int stalledInstructions=1;
int tableNo=0;

MainWindow* obj;
//=====================================================//
Data* Data::getInstance(){
    if(instance==NULL){
        instance=new Data;
    }
    return instance;
}

//Member initializer list && constructor implementation
Data::Data() : R{}, PC(0), Stack{}, SP(0), data{}, dataSize(0), instructions{}, instructionSize(0), nopOccured(false), isLabel(false),
    CLOCK(0), STALL(0), prevRd(-1), prevToPrevRd(-1), FWD_ENABLED(false),
    BRANCH_STALL(false),isPrevLW(false),
    isPrevJmp(false), stallInInstruction(0), MEMSTALL(0), memStallInCurrentInstruction(0), memStallPrev(0), memStallPrevToPrev(0), prevMEM(false), prevToPrevMEM(false)
{
    assemblyInstruction.push_back("jal 0x2");
    assemblyInstruction.push_back("nop");
    cache=new Cache();
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
    nopOccured=false;
    isLabel=false;
    QString a="jal 0x2";
    addCode(a);
    QString b="nop";
    addCode(b);
    CLOCK=0;
    STALL=0;
    prevRd=-1;
    prevToPrevRd=-1;
    //FWD_ENABLED=false;
    BRANCH_STALL=false;
    isPrevLW=false;
    isPrevJmp=false;
    rindex=0;
    cindexPivot=0;
    stalledInstructions=1;
    obj=MainWindow::getInstance();
    assemblyInstruction.clear();
    assemblyInstruction.push_back("jal 0x2");
    assemblyInstruction.push_back("nop");
    MEMSTALL=0;
    memStallInCurrentInstruction=0;
    delete cache;
    cache=new Cache();
    memStallPrev=0;
    memStallPrevToPrev=0;

    prevMEM=false;
    prevToPrevMEM=false;
}

bool isRegisterValid(QString R, bool flag=false)
{
    if(Maps::Registers.contains(R))
    {
        if(flag)
        {
            int i=Maps::Registers[R];
            if(i==0 || i==1)
                return false;
            return true;
        }
        return true;
    }
    return false;
}


bool isVar(QString R){
    Data* D=Data::getInstance();
    return (D->variableMap.contains(R));
}

bool isValidLabel(QString L)
{
    Data* D=Data::getInstance();
    return (D->labelMap.contains(L));
}

bool Data::addCode(QString& text){
    int newInstruction=0;
    int instructionTypeTemplate=8;
    isLabel=false;
    QRegExp sep("(,| |, )");
    QStringList list=text.split(sep);
    int i=0;
    //IF LABEL IS SEPARATELY SEEN ON A LINE IT IS VALID....
    if(list.at(0).contains(":") && labelMap.contains(list.at(0).section(":",0,0))){
        i++;
        if(list.length()==1){
            isLabel=true;
            return true;
        }
    }

    if(Maps::Commands.contains(list.at(i)))
    {
        instructionTypeTemplate=Maps::Commands[list.at(i)].second;
        int IMM=0;
        switch(instructionTypeTemplate)
        {
            case 0:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==3  &&  isRegisterValid(list.at(i), true)&&isRegisterValid(list.at(i+1))&&isRegisterValid(list.at(i+2)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+5+5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+2)]<<(5+5+6));
                }
                else
                    return false;
                break;

            case 1:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==3&&isRegisterValid(list.at(i), true)&&isRegisterValid(list.at(i+1))&&isValue(list.at(i+2)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (16));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+16));
                    IMM=convertToInt(list.at(i+2));
                    if(IMM<0){
                        IMM=IMM & 0x0000ffff;
                    }
                    newInstruction=newInstruction | IMM;
                }
                else
                    return false;
                break;

            case 2:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==3&&isRegisterValid(list.at(i))&&isRegisterValid(list.at(i+1))&&(isValue(list.at(i+2))||isValidLabel(list.at(i+2))))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+16));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (16));
                    if(isValue(list.at(i+2)))
                        newInstruction=newInstruction | convertToInt(list.at(i+2));
                    else
                        newInstruction=newInstruction | (Data::labelMap[list.at(i+2)]-instructionSize);
                }
                else
                    return false;
                break;

            case 3:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==1)
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
                if(list.length()-i==2)
                {
                    if(isRegisterValid(list.at(i), true)){
                        newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (16));
                        if(isVar(list.at(i+1))){
                            newInstruction=newInstruction | (variableMap[list.at(i+1)]);
                            break;
                        }
                        QString base=list.at(i+1).mid(list.at(i+1).indexOf('(')+1, list.at(i+1).indexOf(')')-list.at(i+1).indexOf('(')-1);
                        int offset=convertToInt(list.at(i+1).left(list.at(i+1).indexOf(('('))));
                        //EDITED
                        if(offset<0){
                            //extra 2 bits(from 3) comes becomes of divison by 4(>>2) in next step....
                            //hence the totally obtained piece of code will be 16 bits..
                            offset=offset & 0x0003ffff;
                        }
                        //EDITED
                        if(isRegisterValid(base, true)&&offset%4==0)
                        {
                            newInstruction=newInstruction | (Maps::Registers[base] <<(16+5));
                            newInstruction=newInstruction | (offset >> 2);
                            break;
                        }
                        return false;
                    }
                }
                else
                    return false;
                break;


            case 5:
                newInstruction = newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==1&&isRegisterValid(list.at(i)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+5+5+6));
                }
                else
                    return false;
                break;


            case 6:
                newInstruction=newInstruction | Maps::Commands[list.at(i)].first;
                i++;
                if(list.length()-i==2&&isRegisterValid(list.at(i), true)&& isValue(list.at(i+1)))
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
                if(list.length()-i==3&&isRegisterValid(list.at(i), true)&&isRegisterValid(list.at(i+1))&&isValue(list.at(i+2)))
                {
                    newInstruction=newInstruction | (Maps::Registers[list.at(i)] << (5+6));
                    newInstruction=newInstruction | (Maps::Registers[list.at(i+1)] << (5+5+5+6));
                    newInstruction=newInstruction | convertToInt(list.at(i+1)) << 6;
                }
                else
                    return false;
                break;

            case 8:
                if(list.length()-i==1)
                    newInstruction=0;
                else
                    return false;
            break;
        }
        instructions[instructionSize]=newInstruction;
        instructionSize++;
        return true;
    }
    return false;
}

QString Data::displayRegisters(){
    QString text="";
    text.append(QString("<edit style=\"color:#ffd700\">PC&nbsp;&nbsp;&nbsp;=&nbsp;%1</edit>").arg(PC));
    QStringList regs={"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
                      "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6",  "s7",  "t8",  "t9", "k0", "k1", "gp", "sp", "s8",
                      "ra"};
    if(registerWriteBackLine!=-1)
    {
        for(int i=0;i<registerWriteBackLine;i++)
            text.append(QString("<br>R[%1]&nbsp;&nbsp;$%3 &nbsp;=&nbsp;%2").arg(i).arg(R[i]).arg(regs.at(i)));
        text.append(QString("<br><edit style=\"color:#ffd700\">R[%1]&nbsp;&nbsp;$%3 &nbsp;=&nbsp;%2</edit>").arg(registerWriteBackLine).arg(R[registerWriteBackLine]).arg(regs.at(registerWriteBackLine)));
        for(int i=registerWriteBackLine+1;i<32;i++)
            text.append(QString("<br>R[%1]&nbsp;&nbsp;$%3 &nbsp;=&nbsp;%2").arg(i).arg(R[i]).arg(regs.at(i)));
        registerWriteBackLine=-1;
    }
    else
    {
        for(int i=0;i<32;i++)
            text.append(QString("<br>R[%1]&nbsp;&nbsp;$%3 &nbsp;=&nbsp;%2").arg(i).arg(R[i]).arg(regs.at(i)));
    }
    return text;
}

QString Data::displayData(){
    QString text="";
    for(int i=0;i<dataSize;i++){
        text.append(QString("word %2:    %1\n").arg(data[i]).arg(i+1));
    }
    return text;
}

void Data::updateTable(bool branchStall,bool Jmp_Stall,QTableWidget* timeline)
{
    if(obj->isTimeLineLocked || CLOCK<=0) //This bound we have to change
        return;

    QString vHeader=QString("%1").arg(rindex+1+(tableNo*1000));
    timeline->setVerticalHeaderItem(rindex,new QTableWidgetItem(vHeader));

    int cindex=CLOCK+STALL+MEMSTALLCOUNT-stallInInstruction-1;
    cindex-=cindexPivot;

    if(branchStall)             //guarantees prev was Branch
    {
        vHeader=QString("%1").arg(rindex+1+(tableNo*1000)+1);
        timeline->setVerticalHeaderItem(rindex+1,new QTableWidgetItem(vHeader));
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("IF"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::gray);
        /*
        for(int i=0;i<memStallPrevToPrev;i++)
        {
            timeline->setItem(rindex, cindex++, new QTableWidgetItem("Stall"));
            timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
        }
        */
        if(prevToPrevMEM)
        {
            timeline->setItem(rindex, cindex++, new QTableWidgetItem(QString("Stall x%1").arg(memStallPrevToPrev)));
            timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
            MEMSTALLCOUNT++;
        }

        MEMSTALL+=memStallPrevToPrev;
        memStallPrevToPrev=0;

        timeline->setItem(rindex, cindex, new QTableWidgetItem("Squash"));
        timeline->item(rindex,cindex)->setBackground(Qt::darkRed);
        //NextRow Update and Bound Check
        rindex++;
        if(rindex>=timeline->rowCount())
        {
            cindexPivot=cindex-1;
            obj->setNewTable();
            rindex=0;
            tableNo++;
        }
        //This stallInInstruction corresponds to branch caused stall - Not data dependancy!! Therfore ID/RF comes in next Cycle after IF!!
        stallInInstruction=0;
        incrementLoadDegree();
    }
    timeline->setItem(rindex, cindex++, new QTableWidgetItem("IF"));
    timeline->item(rindex,cindex-1)->setBackground(Qt::gray);
    /*
    for(int i=0;i<memStallPrevToPrev;i++)
    {
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("Stall"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }
    */
    if(prevToPrevMEM)
    {
        timeline->setItem(rindex, cindex++, new QTableWidgetItem(QString("Stall x%1").arg(memStallPrevToPrev)));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }

    if(Jmp_Stall){  //guarantees prev was a jump => memStallPrev=0;
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("IF"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::green);
        stallInInstruction=0;
    }

    timeline->setItem(rindex, cindex++, new QTableWidgetItem("ID/RF"));
    timeline->item(rindex,cindex-1)->setBackground(Qt::red);

    /*
    for(int i=0;i<memStallPrev;i++)
    {
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("Stall"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }
    */
    if(prevMEM)
    {
        timeline->setItem(rindex, cindex++, new QTableWidgetItem(QString("Stall x%1").arg(memStallPrev)));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }

    for(int i=0;i<stallInInstruction;i++)
    {
        if(i==stallInInstruction-1){
            timeline->setItem(rindex, cindex++, new QTableWidgetItem("ID/RF"));
            timeline->item(rindex,cindex-1)->setBackground(Qt::red);
            break;
        }
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("Stall"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }


    timeline->setItem(rindex, cindex++, new QTableWidgetItem("EX"));
    timeline->item(rindex,cindex-1)->setBackground(Qt::darkCyan);
    /*
    for(int i=0;i<memStallInCurrentInstruction;i++){
        timeline->setItem(rindex, cindex++, new QTableWidgetItem("MEMStall"));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }
    */
    if(memStallInCurrentInstruction>0)
    {
        timeline->setItem(rindex, cindex++, new QTableWidgetItem(QString("MEMStall x%1").arg(memStallInCurrentInstruction)));
        timeline->item(rindex,cindex-1)->setBackground(Qt::darkGreen);
    }
    timeline->setItem(rindex, cindex++, new QTableWidgetItem("MEM"));
    timeline->item(rindex,cindex-1)->setBackground(Qt::magenta);
    timeline->setItem(rindex, cindex++, new QTableWidgetItem("WB"));
    timeline->item(rindex,cindex-1)->setBackground(Qt::red);

    //NextRow Update and Bound Check
    rindex++;
    if(rindex>=timeline->rowCount())
    {
        cindexPivot=cindex-4-stallInInstruction;
        obj->setNewTable();
        rindex=0;
        tableNo++;
    }
    /*
    if(memStallPrev>0 && stallInInstruction>0){
        MEMSTALL+=memStallPrev;
        memStallPrev=0;
    }
    */
    if(prevMEM && stallInInstruction>0)
    {
        MEMSTALLCOUNT+=1;
        MEMSTALL+=memStallPrev;
        prevMEM=false;
        memStallPrev=0;
    }
}

void Data::updateStallList(int CurrentInstructionCounter,QListWidget *stallList){
    stallList->addItem(QString("%3. %1  -%2 Stalls ").arg(assemblyInstruction.at(CurrentInstructionCounter),-35).arg(stallInInstruction).arg(stalledInstructions));
    stalledInstructions++;
}

QString Data::forConsole(){
    QString text="";
    int temp=MEMSTALL+memStallPrev+memStallPrevToPrev+memStallInCurrentInstruction;
    text.append(QString("No of instructions executed: %1").arg(QString::number(rindex)));
    text.append(QString("<br>No of Clock Cycles in total: %1").arg(QString::number(CLOCK+STALL+4+temp)));
    text.append(QString("<br>No of Stalls in total: %1").arg(QString::number(STALL)));
    text.append(QString("<br>No of memStalls in total: %1").arg(QString::number(temp)));
    if(CLOCK!=0){
        float x=((float)rindex)/((float)(CLOCK+STALL+4+temp));
        text.append(QString("<br>Instructions Per ClockCycle <edit style=\"color:#ffd700\">(IPC) : %1</edit>").arg(x));
    }
    text.append(QString("<br><br><edit style=\"color:#ffd700\">Cache</edit><br>"));
    text.append(QString("(<edit style=\"color:#66ff66\">Green : No of Hits</edit>  -  <edit style=\"color:#ff4d4d\">Red : No of Misses</edit>  -  White : Average Access Time)"));
    int noOfMisses=cache->getMisses();
    int noOfHits=cache->getHits();
    int noOfAccesses=noOfHits+noOfMisses;
    if(noOfAccesses!=0)
    {
        float missRate=noOfMisses/(noOfAccesses*1.0);
        float averageAccessTime=cache->getHitTime()*noOfAccesses+cache->getMissPenalty()*missRate;
        text.append(QString("<br>Overall  : <edit style=\"color:#66ff66\">%2  </edit><edit style=\"color:#ff4d4d\">%3  </edit>%4").arg(noOfHits).arg(noOfMisses).arg(averageAccessTime));
    }
    else
        text.append(QString("<br><edit style=\"color:#ff4d4d\">No Accesses Issued to Cache</edit>"));
    if(noOfAccesses==0)
        return text;
    int noOfSets=cache->getNoOfSets();
    for(int i=0;i<noOfSets;i++)
    {
        Set temp=*(cache->getSet(i));
        int noOfMisses=temp.getMisses();
        int noOfHits=temp.getHits();
        int noOfAccesses=noOfHits+noOfMisses;
        if(noOfAccesses!=0)
        {
            float missRate=noOfMisses/(noOfAccesses*1.0);
            float averageAccessTime=cache->getHitTime()*noOfAccesses+cache->getMissPenalty()*missRate;
            text.append(QString("<br><edit style=\"color:#ffd700\">Set %1  </edit><edit style=\"color:#66ff66\">%2  </edit><edit style=\"color:#ff4d4d\">%3  </edit>%4").arg(i+1).arg(noOfHits).arg(noOfMisses).arg(averageAccessTime));
        }
        //Or Choose to not display Set Number can be Huge
        //else
        //    text.append(QString("<br><edit style=\"color:#ffd700\">Set %1  </edit><edit style=\"color:#ff4d4d\">No Accesses Issued</edit>").arg(i+1));
    }
    return text;
}

bool Data::run(QTableWidget **timeline,QListWidget *stallList){
    while(PC<instructionSize && !nopOccured){
        CLOCK++;
        int CIC=PC;//CURRENT INSTRUCTION COUNTER;
        bool branch_stall=BRANCH_STALL;
        bool Jmp_Stall=isPrevJmp;
        stallInInstruction=0;
        incrementLoadDegree();
        int instruction=instructionFetch();
        instructionDecodeRegisterFetch(instruction);
        if(stallInInstruction>0 &&(CLOCK+STALL<2000))
            updateStallList(CIC,stallList);
        updateTable(branch_stall,Jmp_Stall,timeline[obj->tableIndex]);

    }
    return nopOccured;
}

bool Data::runStepByStep(QTableWidget **timeline,QListWidget *stallList){
    if(PC<instructionSize && !nopOccured){
        CLOCK++;
        bool branch_stall=BRANCH_STALL;
        bool Jmp_Stall=isPrevJmp;
        int CIC=PC;//CURRENT INSTRUCTION COUNTER;
        stallInInstruction=0;
        incrementLoadDegree();
        int instruction=instructionFetch();
        instructionDecodeRegisterFetch(instruction);
        if(stallInInstruction>0 &&(CLOCK+STALL<2000))
            updateStallList(CIC,stallList);
        updateTable(branch_stall,Jmp_Stall,timeline[obj->tableIndex]);
    }
    return PC<instructionSize;
}

int Data::instructionFetch(){
    int result=instructions[PC];
    PC++;
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
        if(BRANCH_STALL){
            STALL++;
            stallInInstruction=1;
            BRANCH_STALL=false;
        }
        else if(isPrevJmp){
            STALL++;
            stallInInstruction=1;
            isPrevJmp=false;
        }
        else if(FWD_ENABLED && isPrevLW && (Rs==prevRd || Rt==prevRd))      //dependancy cases
        {
            STALL+=1;
            stallInInstruction=1;
            isPrevLW=false;
            prevRd=-1;      //For 3 instructions I1 I2 I3... if I2 and I3 are dependent on I3, I2 will stall and make sure I3 can execute normally
        }
        else if(!FWD_ENABLED && (Rs==prevRd || Rt==prevRd)){
            STALL+=2;
            stallInInstruction=2;
            prevRd=-1;
        }
        else if(!FWD_ENABLED && (Rs==prevToPrevRd || Rt==prevToPrevRd)){
            STALL+=1;
            stallInInstruction=1;
        }
        prevToPrevRd=prevRd;
        prevRd=Rd;
        Execute(funct,R[Rs],R[Rt],Rd,shamt);
    }
    //J - Type instruction
    else if(opCode==0x2  || opCode==0x3){
        int target=instruction & 0x3ffffff;
        if(BRANCH_STALL){
            STALL++;
            stallInInstruction=1;
            BRANCH_STALL=false;
        }
        else if(isPrevJmp){
            STALL++;
            stallInInstruction=1;
            isPrevJmp=false;
        }
        prevToPrevRd=prevRd;
        prevRd=-1;
        Execute(opCode,target);
    }
    //I - Type
    else if(opCode==0x8 || opCode==0xc ||opCode==0xd ||opCode==0x5 || opCode==0x4
            ||opCode==0xa || opCode==0xf ||opCode==0x23 ||opCode==0x2b){
        int Rs=(instruction>>21) & 0x1f;
        int Rt=(instruction>>16) & 0x1f;
        int immediate=instruction & 0xffff;
        //IF IMMEDIATE WAS A NEGATIVE NUMBER...
        if(((immediate>>15) & 1)==1){
            immediate=immediate | 0xffff0000;
        }

        if(BRANCH_STALL){
            STALL++;
            stallInInstruction=1;
            BRANCH_STALL=false;
        }
        else if(isPrevJmp){
            STALL++;
            stallInInstruction=1;
            isPrevJmp=false;
        }
        else if(FWD_ENABLED && isPrevLW && Rs==prevRd)
        {
            STALL+=1;
            stallInInstruction=1;
            isPrevLW=false;
            prevToPrevRd=-1;
        }
        else if(!FWD_ENABLED && Rs==prevRd){
            STALL+=2;
            stallInInstruction=2;
            prevRd=-1;
        }
        else if(!FWD_ENABLED && Rs==prevToPrevRd){
            STALL++;
            stallInInstruction=1;
        }
        prevToPrevRd=prevRd;
        prevRd=Rt;

        Execute(opCode,R[Rs],Rt,immediate);
    }

}

//R- TYPE
void Data::Execute(int funct,int Rs,int Rt,int Rd,int shamt){
    int result=0;
    switch(funct){
    case 0x0: //sll
        if(Rd==0){
            break;
        }
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
        Rd=-1;
        isPrevJmp=true;
        break;

    case 0x24://and
        result=Rs & Rd;
        break;

    case 0x5://or
        result=Rs | Rd;
        break;
    }
    MEM(funct,Rd,result);
}

//I-TYPE
void Data::Execute(int opCode,int R1,int R2,int immediate){
    //R2 is index of the destination Register
    //R1 is index of source Register
    int result=0;
    switch(opCode){
    case 0x8:{//addi
        int r1=R1;
        result=r1+immediate;
        break;
    }
    case 0xc:{//andi
        int r1=R1;
        result=r1&immediate;
        break;
    }
    case 0xd:{//ori
        int r1=R1;
        result=r1|immediate;
        break;
    }
    case 0x5:{//bne
        int r1=R1;
        int r2=R[R2];
        if(r1!=r2){
            result=PC+immediate-1;
            BRANCH_STALL=true;
        }
        else
            result=PC;
        break;
    }
    case 0x4:{//beq
        int r1=R1;
        int r2=R[R2];
        if(r1==r2){
            result=PC+immediate-1;
            BRANCH_STALL=true;
        }
        else
            result=PC;
        break;
    }
    case 0xa:{//slti
        int r1=R1;
        if(r1<immediate)
            result=1;
        else
            result=0;
        break;
    }
    case 0xf://lui
        result=immediate<<16;
        //should be stored in R[R2]
        break;

    case 0x23:{//lw
        int r1=R1;
        result=immediate+r1;
        isPrevLW=true;

        if(cache->checkHit(result*4)){
            memStallInCurrentInstruction = 1;

        }else{
            memStallInCurrentInstruction = 101;
            cache->storeInCache(result*4);
        }
        break;
    }
    case 0x2b:{//sw
        int r1=R1;
        result=immediate+r1;
        break;
    }   
    }
    MEM(opCode, R2, result);
}

//J TYPE
void Data::Execute(int opCode,int target){
    int Rd=-1;
    if(opCode==0x2){//Jump
        Rd=-1;
    }
    if(opCode==0x3){//jal
        Rd=31;
    }
    isPrevJmp=true;
    MEM(opCode,Rd,target);
}

void Data::MEM(int opCode, int Rd, int result)
{
    //Rd is index of the destination Register
    int value=0;
    switch(opCode){
    case 0x5:{//bne
        PC=result;
        WB(-1, result);
        break;
    }
    case 0x4:{//beq
        PC=result;
        WB(-1, result);
        break;
    }
    case 0x23:{//lw
        if(result<(int)(sizeof(data)/sizeof(data[0])))//BoundCheck
            value=data[result];
        else
            Rd=-1;
        WB(Rd, value);
        break;
        //value must be written to Register Rd in WB
    }
    case 0x2b:{//sw
        if(result<(int)(sizeof(data)/sizeof(data[0])))//BoundCheck
        {
            value=-1;
            data[result]=R[Rd];
            WB(value, result);
        }
        break;
    }
    case 0x3://jal
        value=PC;
        PC=result;
        WB(Rd,value);
        break;
    case 0x2://jump
        PC=result;
        WB(Rd,result);
        break;

    //funct of jr is 0x8 and opCode of addi is 0x8.....
    case 0x8:
        //jr has Rd==-1.....addi and jr is distinguished as such
        if(Rd==-1){
            PC=result;
            WB(-1,result);
            break;
        }
        WB(Rd,result);
        break;
    default:
        WB(Rd,result);
    }


}
void Data::WB(int Rd, int result)
{
    if(Rd==0){
        nopOccured=true;
    }
    if(2<=Rd && Rd<32){
        R[Rd]=result;
        registerWriteBackLine=Rd;
    }
}
