#ifndef CACHE_H
#define CACHE_H

class Block
{
    private:
    bool dirtyBit;
    bool validBit;
    int tag;
    int lru;
    public:
        Block();
        bool isModified();
        int getTag();
        void setTag(int x);
        bool getValidBit();
        void setValidBit(bool x);
        bool getDirtyBit();
        void setDirtyBit(bool x);
        int getLRU();
        void setLRU(int Clock);
        void setNewBlock(int tag, int& setClock);
};
class Set
{
    private:
        Block** blocks;
        int noOfBlocks;
        int setClock;
        int noOfMisses;
        int noOfHits;
    public:
    Set(int noOfBlocks);
    bool checkHit(int tag, int offset);
    bool setBlock(int tag, int offset);
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
