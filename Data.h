#ifndef DATA_H
#define DATA_H
#include <QString>
#include<QBitArray>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QTableWidget>
#include <QListWidget>

class Data{
public:
    //PHASE 1 COMPONENTS
    int R[32];
    int PC;
    int Stack[2048];
    int SP;
    int data[4096];
    int dataSize;
    int instructions[4096];
    int instructionSize;
    QStringList assemblyInstruction;
    QMap<QString,int> labelMap;
    QMap<QString,int> variableMap;
    bool nopOccured; //EXIT CONDITION
    bool isLabel;

    //PHASE 2 COMPONENTS
    int CLOCK;
    int STALL;
    int prevRd;
    int prevToPrevRd;
    bool FWD_ENABLED;
    bool BRANCH_STALL; //WHETHER THE CURRENT INS WOULD CAUSE A BRANCH RELATED STALL FOR NEXT INSTRUCTION (NOT TAKEN PREDICTOR)
    bool isPrevLW;
    bool isPrevJmp;
    int stallInInstruction;
    //We won't consider 3 stalls for normal data dependancy - (by making WB - half cycle)

private:
    static Data* instance;
    Data();

public:
    static Data* getInstance();
    void initialize();
    bool addCode(QString& data);
    QString displayRegisters();
    QString displayData();
    bool run(QTableWidget **timeline,QListWidget *stallList);
    bool runStepByStep(QTableWidget **timeline,QListWidget *stallList);
    void updateTable(bool branchStall,bool jumpStall,QTableWidget *timeline);
    void updateStallList(int CurrentInstructionCounter,QListWidget *stallList);
    QString getTimeLine();
    QString forConsole();

    int instructionFetch();
    void instructionDecodeRegisterFetch(int instruction);
    //OVERLOADING EXECUTE FUNCTION FOR I-TYPE, R-TYPE AND J-TYPE
    void Execute(int opCode,int target);
    void Execute(int funct,int Rs,int Rt,int Rd,int shamt);
    void Execute(int opCode,int Rs,int Rt,int immediate);
    void MEM(int opCode, int R2, int result);
    void WB(int address, int value);

signals:
    void UpdateStallList(int CurrentInstructionCounter);

};



#endif // DATA_H
