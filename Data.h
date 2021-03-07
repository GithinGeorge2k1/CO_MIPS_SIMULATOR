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
    QMap<QString,int> labelMap;
private:
    static Data* instance;
    Data();
public:
    static Data* getInstance();
    void initialize();
    bool addCode(QString& data);
    QString displayRegisters();
};



#endif // DATA_H
