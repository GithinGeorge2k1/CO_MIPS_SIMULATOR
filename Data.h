#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QStringList>

class Data{
    int R[31];
    int PC;
    int Stack[2048];
    int data[65536];
    int Text[4096];
public:
    Data();
    bool addCode(QString data);
    QString displayRegisters();
};

#endif // DATA_H
