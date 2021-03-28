#include "utilities.h"
#include <QRegExp>

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
        QRegExp exp("[-+]?[0-9]*");
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

QString decimalToHex(int x){
    QString result="";
    unsigned int n=(unsigned int)x;
    while(n!=0){
        unsigned int temp=n%16;
        char a;
        if(temp<10){
            a=temp+48;
        }
        else{
            a=temp+55;
        }
        result=QString(a)+result;
        n=n/16;
    }
    return result;
}

