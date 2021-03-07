#ifndef DATA_H
#define DATA_H
#include <QString>
class Data{
    int R[31];
    int PC;
    int Stack[2048];
    int data[65536];
    int Text[4096];
public:
    Data();
    void addCode(QString data);
};

#endif // DATA_H
