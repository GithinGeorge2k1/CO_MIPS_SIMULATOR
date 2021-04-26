#ifndef CACHE_H
#define CACHE_H

class Block
{
    private:
    int dirtyBit;
    int tag;
    public:
        Block();
};
class Set
{
    private:
        Block** blocks;
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
    public:
        bool valid;
    Cache();
    void setCache(int cacheSize, int blockSize, int associativity);
    bool checkHit(int address);
};

#endif // CACHE_H
