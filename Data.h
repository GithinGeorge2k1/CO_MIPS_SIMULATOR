#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QStringList>
#include <QMap>

class Data{
public:
    int R[31];
    int PC;
    int Stack[2048];
    int SP;
    int data[65536];
    int dataSize;
    int instructions[4096];
    int instructionSize;
    QMap<QString,int> labelMap;
    QMap<QString,int> variableMap;
private:
    static Data* instance;
    Data();
public:
    static Data* getInstance(); 
    void initialize();
    bool addCode(QString& data, int currentLineNo);
    QString displayRegisters();
    QString displayData();
};



#endif // DATA_H
