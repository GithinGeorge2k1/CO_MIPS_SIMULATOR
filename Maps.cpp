#include "Maps.h"

QMap<QString,QPair<int,int>> Maps::Commands;
QMap<QString,uint> Maps::Registers;
Maps* Maps::KeyValueMap=NULL;
Maps::Maps()
{
    //Command Templates
//    0 for inst R1 R2 R3           = add,sub,slt,and,or
//    1 for inst R1 R2 value        = andi,ori,addi,slti
//    2 for inst R1 R2 value|Label  = bne,beq               /////exe   value | Label    4, labelmap-currentlineNo;;
//    3 for inst value|Label        = j,jal                 /////exe   value | Label    value|Label;
//    4 for inst R1 x(R2)           = lw,sw                 /////exe   x->x/4
//    5 for inst R1                 = jr
//    6 for inst R1 value|Label     = lui
//    7 for inst R1 R2 R3           = sll,srl (shamt) is used

    Commands["add"]=qMakePair(0x20,0);
    Commands["sub"]=qMakePair(0x22,0);
    Commands["addi"]=qMakePair(0x20000000,1);

    Commands["and"]=qMakePair(0x24,0);
    Commands["andi"]=qMakePair(0x20000000,1);
    Commands["or"]=qMakePair(0x5,0);
    Commands["ori"]=qMakePair(0x34000000,1);
    Commands["sll"]=qMakePair(0x0,7);
    Commands["srl"]=qMakePair(0x2,7);

    Commands["bne"]=qMakePair(0x14000000,2);
    Commands["beq"]=qMakePair(0x10000000,2);
    Commands["slt"]=qMakePair(0xa,0);
    Commands["slti"]=qMakePair(0x28000000,1);

    Commands["lui"]=qMakePair(0x3c000000,6);
    Commands["lw"]=qMakePair(0x8c000000,4);
    Commands["sw"]=qMakePair(0xac000000,4);

    Commands["j"]=qMakePair(0x08000000,3);
    Commands["jr"]=qMakePair(0x8,5);
    Commands["jal"]=qMakePair(0x0c000000,3);

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
}
Maps* Maps::getInstance()
{
    if(KeyValueMap==NULL)
        KeyValueMap=new Maps();
    return KeyValueMap;
}




