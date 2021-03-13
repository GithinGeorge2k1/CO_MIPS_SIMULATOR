#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QStringList>
#include <QMap>

class Data{
public:
    int R[32];
    int PC;
    int Stack[2048];
    int SP;
    int data[65536];
    int dataSize;
    int instructions[4096];
    int instructionSize;
    QMap<QString,int> labelMap;
    QMap<QString,int> variableMap;
    bool nopOccured;
private:
    static Data* instance;
    Data();

public:
    static Data* getInstance(); 
    void initialize();
    bool addCode(QString& data);
    QString displayRegisters();
    QString displayData();
    bool run();
    bool runStepByStep();

    int instructionFetch();
    void instructionDecodeRegisterFetch(int instruction);
    void Execute(int opCode,int target);
    void Execute(int funct,int Rs,int Rt,int Rd,int shamt);
    void Execute(int opCode,int Rs,int Rt,int immediate);


    void MEM(int opCode, int R2, int result);
    void WB(int address, int value);
};



#endif // DATA_H
