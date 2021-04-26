#include "cache.h"
#include<QtMath>
int power(int a, int b)//a^b
{
    if(b==0)
        return 1;
    return a*power(a, b-1);
}

LRU::LRU()
{
    lastAccessClock=-1;
    noOfAcceses=0;
}

Block::Block(int validBit, int tag)
{
    dirtyBit=0;
    this->validBit=validBit;
    this->tag=tag;
}
int Block::getTag()
{
    return tag;
}
bool Block::isModified()
{
    if(dirtyBit==1)
        return true;
    return false;
}
Set::Set(int noOfBlocks)
{
    blocks=new Block*[noOfBlocks];
    LRUParams=new LRU*[noOfBlocks];
    for(int i=0;i<noOfBlocks;i++)
    {
        blocks[i]=new Block();
        LRUParams[i]=new LRU();
    }
}

bool Set::checkHit(int tag, int offset)
{
    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
        return false;
    for(int i=0;i<noOfBlocks;i++)
        if(tag==blocks[i]->getTag())
            return true;
    return false;
}
bool Set::setBlock(int tag, int offset)
{
    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
        return false;
    int kickOutIndex=0;
    for(int i=0;i<noOfBlocks;i++)
    {
        if(LRUParams[i]->lastAccessClock==-1)
            {kickOutIndex=i;break;}//this case nothing is kicked out
        if(LRUParams[i]->lastAccessClock<LRUParams[kickOutIndex]->lastAccessClock)
            kickOutIndex=i;
        else if(LRUParams[i]->lastAccessClock==LRUParams[kickOutIndex]->lastAccessClock)// I don't think this case occurs, but let it be
            if(LRUParams[kickOutIndex]->noOfAcceses<LRUParams[i]->noOfAcceses)
                kickOutIndex=i;
    }
    if(blocks[kickOutIndex]->isModified())
    {
        //WB policy
    }
    Block* temp=blocks[kickOutIndex];
    blocks[kickOutIndex]=new Block(1, tag);
    delete temp;
}

Cache::Cache() : addressableSize(4), valid(false) //32 Bit ~ 4 Byte addressable machine - Assumption
{

}

void Cache::setCache(int cacheSize, int blockSize, int associativity)
{
    valid=true;
    int totalBlocks=(cacheSize*1024)/blockSize; // cacheSize in KB and blockSize in Bytes
    int noOfWords=blockSize/addressableSize; // Word per Block
    noOfSets=totalBlocks/associativity;


    bits_offset=qFloor(qLn(noOfWords)/qLn(2));// Idk if converting real values to int can give any serious problems / otherwise can use isPowerOfTwo I changed it to return log2(x)
    bits_index=qFloor(qLn(noOfSets)/qLn(2));
    bits_tag=32-bits_index-bits_offset;

    sets=new Set*[noOfSets];
    for(int i=0;i<noOfSets;i++)
    {
        sets[i]=new Set(associativity);
    }
}

bool Cache::storeInCache(int address)
{
    int andValue;
    andValue=power(2, bits_offset)-1;//Not Required
    int offset=address & andValue;//Not Required

    andValue=power(2, bits_index)-1;
    int index=(address>>bits_offset) & andValue;

    andValue=power(2, bits_tag)-1;
    int tag=(address>>(bits_index+bits_offset)) & andValue;

    if(index>=0 && index<noOfSets)
        return sets[index]->setBlock(tag, offset);
    return false;
}
bool Cache::checkHit(int address)
{
    int andValue;
    andValue=power(2, bits_offset)-1;//Not Required
    int offset=address & andValue;//Not Required

    andValue=power(2, bits_index)-1;
    int index=(address>>bits_offset) & andValue;

    andValue=power(2, bits_tag)-1;
    int tag=(address>>(bits_index+bits_offset)) & andValue;

    if(index>=0 && index<noOfSets)
        return sets[index]->checkHit(tag, offset);
    return false;
}
