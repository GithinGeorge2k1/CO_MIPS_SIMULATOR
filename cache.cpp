#include "cache.h"
#include<QtMath>
#include <QDebug>

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
<<<<<<< Updated upstream
Set::Set(int noOfBlocks)  : noOfMisses(0), noOfHits(0)
=======

Set::Set(int noOfBlocks)
>>>>>>> Stashed changes
{
    setClock=0;
    noOfHits=0;
    noOfMisses=0;
    blocks=new Block*[noOfBlocks];
    for(int i=0;i<noOfBlocks;i++)
    {
        blocks[i]=new Block();
    }
    this->noOfBlocks=noOfBlocks;
}
int Set::getHits()
{
    return noOfHits;
}
int Set::getMisses()
{
    return noOfMisses;
}
bool Set::checkHit(int tag, int offset)
{
//    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
//        return false;
    for(int i=0;i<noOfBlocks;i++){
        if(tag==blocks[i]->getTag()){
            noOfHits++;
            qDebug()<<"cacheHit";
            return true;
        }
    }
    qDebug()<<"cacheMiss";
    noOfMisses++;
    return false;
}
bool Set::setBlock(int tag, int offset)
{
//    if(offset<0 || offset>=noOfBlocks)//Only for safety nothing important
//        return false;
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

Cache::Cache() : addressableSize(4), noOfHits(0), noOfMisses(0),hitTime(2), missPenalty(100), valid(false)  //32 Bit ~ 4 Byte addressable machine - Assumption
{

}
Set* Cache::getSet(int i)
{
    return sets[i];
}
int Cache::getNoOfSets()
{
    return noOfSets;
}
int Cache::getHits()
{
    return noOfHits;
}
int Cache::getMisses()
{
    return noOfMisses;
}
int Cache::getHitTime()
{
    return hitTime;
}
int Cache::getMissPenalty()
{
    return missPenalty;
}
void Cache::setCache(int cacheSize, int blockSize, int associativity)
{
    valid=true;
    int totalBlocks=(cacheSize*1024)/blockSize;     // cacheSize in KB and blockSize in Bytes
    //int noOfWords=blockSize/addressableSize;        // Word per Block
    noOfSets=totalBlocks/associativity;


    bits_offset=(int)log2(blockSize);// Idk if converting real values to int can give any serious problems / otherwise can use isPowerOfTwo I changed it to return log2(x)
    bits_index=(int)log2(noOfSets);

    qDebug()<<"Offset bits: "<< bits_offset<< "\n";
    qDebug()<<"index bits: "<< bits_index;


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
    {
        if(sets[index]->checkHit(tag, offset))
        {
            noOfHits++;
            return true;
        }
    }
    noOfMisses++;
        return false;
}
