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
    lru=-1;
    dirtyBit=false;
    this->validBit=false;
    this->tag=-1;
}
int Block::getTag()
{
    return tag;
}
bool Block::isModified()
{
    return dirtyBit;
}
Set::Set(int noOfBlocks)
{
    setClock=0;
    blocks=new Block*[noOfBlocks];
    for(int i=0;i<noOfBlocks;i++)
    {
        blocks[i]=new Block();
    }
}

bool Set::checkHit(int tag, int offset)
{
    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
        return false;
    for(int i=0;i<noOfBlocks;i++){
        if(tag==blocks[i]->getTag()){
            noOfHits++;
            return true;
        }
    }
    noOfMisses++;
    return false;
}
bool Set::setBlock(int tag, int offset)
{
    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
        return false;
    int kickOutIndex=0;
    for(int i=0;i<noOfBlocks;i++)
    {
        if(!blocks[i]->getValidBit()){
            kickOutIndex=i;
            break;
        }
        if(blocks[i]->getLru()<blocks[kickOutIndex]->getLru()){
            kickOutIndex=i;
        }
    }
    if(blocks[kickOutIndex]->isModified())
    {
        //WB policy
        //Maybe Take No Of Writes
    }
    blocks[kickOutIndex]->setTag(tag);
    blocks[kickOutIndex]->setDirtyBit(false);
    blocks[kickOutIndex]->setLru(setClock);
    setClock++;
    return true;
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
