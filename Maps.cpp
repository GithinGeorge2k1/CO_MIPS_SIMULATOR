#include "Maps.h"

QMap<QString,uint> Maps::Commands;
QMap<QString,uint> Maps::Registers;
Maps* Maps::KeyValueMap=NULL;
Maps::Maps()
{
    Commands["add"]=0x20;
    Commands["sub"]=0x22;
    Commands["addi"]=0x20000000;

    Commands["and"]=0x24;
    Commands["andi"]=0x20000000;
    Commands["or"]=0x5;
    Commands["ori"]=0x34000000;
    Commands["sll"]=0x0;
    Commands["slr"]=0x2;

    Commands["bne"]=0x14000000;
    Commands["beq"]=0x10000000;
    Commands["slt"]=0xa;
    Commands["slti"]=0x28000000;

    Commands["lui"]=0x3c000000;
    Commands["lw"]=0x8c000000;
    Commands["sw"]=0xac000000;

    Commands["j"]=0x08000000;
    Commands["jr"]=0x8;
    Commands["jal"]=0x0c000000;

    Registers["$r0"]=0x0;
    Registers["$at"]=0x1;
    Registers["$v0"]=0x2;
    Registers["$v1"]=0x3;
    Registers["$a0"]=0x4;
    Registers["$a1"]=0x5;
    Registers["$a2"]=0x6;
    Registers["$a3"]=0x7;
    Registers["$t0"]=0x8;
    Registers["$t1"]=0x9;
    Registers["$t2"]=0xa;
    Registers["$t3"]=0xb;
    Registers["$t4"]=0xc;
    Registers["$t5"]=0xd;
    Registers["$t6"]=0xe;
    Registers["$t7"]=0xf;
    Registers["$s0"]=0x10;
    Registers["$s1"]=0x11;
    Registers["$s2"]=0x12;
    Registers["$s3"]=0x13;
    Registers["$s4"]=0x14;
    Registers["$s5"]=0x15;
    Registers["$s6"]=0x16;
    Registers["$s7"]=0x17;
    Registers["$t8"]=0x18;
    Registers["$t9"]=0x19;
    Registers["$k0"]=0x1a;
    Registers["$k1"]=0x1b;
    Registers["$gp"]=0x1c;
    Registers["$sp"]=0x1d;
    Registers["$s8"]=0x1e;
    Registers["$ra"]=0x1f;
//    QString path=":/MapCodes/Commands.csv";
//    QFile commands(path);
//    QTextStream fin1(&commands);
//    while(!fin1.atEnd())
//    {
//        QString line=fin1.readLine().simplified();
//        QStringList keyvaluepair=line.split(",0x");
//        Commands.insert(keyvaluepair.at(0), keyvaluepair.at(1).toUInt());
//    }
//    commands.close();
//    path=":/MapCodes/Registers.csv";
//    QFile registers(path);
//    QTextStream fin2 (&registers);
//    while(!fin2.atEnd())
//    {
//        QString line=fin2.readLine().simplified();
//        QStringList keyvaluepair=line.split(",0x");
//        Registers.insert(keyvaluepair.at(0), keyvaluepair.at(1).toUInt());
//    }
//    registers.close();
}
Maps* Maps::getInstance()
{
    if(KeyValueMap==NULL)
        KeyValueMap=new Maps();
    return KeyValueMap;
}




