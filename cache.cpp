#include "cache.h"
#include<QtMath>
int power(int a, int b)//a^b
{
    if(b==0)
        return 1;
    return a*power(a, b-1);
}
Block::Block()
{
    dirtyBit=0;
    tag=-1;
}
Set::Set(int noOfBlocks)
{
    blocks=new Block*[noOfBlocks];
    for(int i=0;i<noOfBlocks;i++)
    {
        blocks[i]=new Block();
    }
}
bool Set::checkHit(int tag, int offset)
{
    if(offset>=0 && offset<noOfBlocks)
    {

    }
    return false;
}

Cache::Cache() : valid(false)
{

}
void Cache::setCache(int cacheSize, int blockSize, int associativity)
{
    valid=true;
    int totalBlocks=(cacheSize*1024)/blockSize; // cacheSize in KB and blockSize in Bytes
    noOfSets=totalBlocks/associativity;

    bits_offset=qFloor(qLn(associativity)/qLn(2));
    bits_index=qFloor(qLn(noOfSets)/qLn(2));
    bits_tag=32-bits_index-bits_offset;

    sets=new Set*[noOfSets];
    for(int i=0;i<noOfSets;i++)
    {
        sets[i]=new Set(associativity);
    }
}
bool Cache::checkHit(int address)
{
    int andValue;
    andValue=power(2, bits_offset)-1;
    int offset=address & andValue;

    andValue=power(2, bits_index)-1;
    int index=(address>>bits_offset) & andValue;

    andValue=power(2, bits_tag)-1;
    int tag=(address>>(bits_index+bits_offset)) & andValue;

    if(index>=0 && index<noOfSets)
        return sets[index]->checkHit(tag, offset);
    return false;
}
