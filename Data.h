#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QStringList>

class Data{
    int R[31];
    int PC;
    int Stack[2048];
    int SP;
    int data[65536];
    int dataSize;
    int instructions[4096];
    int instructionSize;
public:
    Data();
    void initialize();
    bool addCode(QString data);
    QString displayRegisters();
};

#endif // DATA_H
