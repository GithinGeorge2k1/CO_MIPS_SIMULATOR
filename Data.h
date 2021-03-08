#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QStringList>
#include <QMap>

class Data{
    int R[31];
    int PC;
    int Stack[2048];
    int SP;
    int data[65536];
    int dataSize;
    int instructions[4096];
    int instructionSize;
private:
    static Data* instance;
    Data();
public:
    QMap<QString,int> labelMap;
    static Data* getInstance(); 
    void initialize();
    bool addCode(QString& data, int currentLineNo);
    QString displayRegisters();
};



#endif // DATA_H
