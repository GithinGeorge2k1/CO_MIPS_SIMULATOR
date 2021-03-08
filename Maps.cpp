#include "Maps.h"

QMap<QString,int> Maps::Commands;
QMap<QString,int> Maps::Registers;

Maps::Maps(){
    //
    Commands["add"]=0x1000000;



    //
    Registers["$zer0"]=0x00000000;
}




