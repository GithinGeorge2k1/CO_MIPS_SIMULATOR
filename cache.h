#ifndef CACHE_H
#define CACHE_H

struct LRU
{
    int lastAccessClock; //can be useful if 2 Blocks have the same noOfAccesses made
    int noOfAcceses;
    LRU();
};
class Block
{
    private:
    int dirtyBit;
    int validBit;
    int tag;
    public:
        Block();
        int getTag();
};
class Set
{
    private:
        Block** blocks;
        LRU** LRUParams;
        int noOfBlocks;
    public:
    Set(int noOfBlocks);
    bool checkHit(int tag, int offset);
};
class Cache
{
    private:
        Set** sets;
        int noOfSets;
        int bits_offset;
        int bits_index;
        int bits_tag;
        int addressableSize;
    public:
        bool valid;
    Cache();
    void setCache(int cacheSize, int blockSize, int associativity);
    bool checkHit(int address);
    bool storeInCache(int address);
};

#endif // CACHE_H
