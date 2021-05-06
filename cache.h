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
        int getTag();
        bool isModified();

        bool getValidBit() const{
            return validBit;
        }

        bool getDirtyBit() const{
            return dirtyBit;
        }

        int getLru() const{
            return lru;
        }

        void setLru(int Clock){
            lru=Clock;
        }

        void setDirtyBit(bool x) {
            dirtyBit=x;
        }

        void setValidBit(bool x){
            validBit=x;
        }

        void setTag(int x) {
            tag=x;
        }
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
    bool checkHit(int tag);
    bool setBlock(int tag);
    int getHits();
    int getMisses();

    //For multiLevelCache
    int lruBlock();
    int kickedOutTag();
    void inValidateBlock(int tag);
};
class Cache
{
    private:
        Set** sets;
        int noOfSets;
        int bits_offset;
        int bits_index;
        int bits_tag;
        int noOfHits;
        int noOfMisses;
        int missPenalty;
        int storeLatency;
        int loadLatency;
    public:
        bool valid;

    Cache();
    void setCache(int cacheSize, int blockSize, int associativity, int loadLatency, int storeLatency);
    bool checkHit(int address);
    bool storeInCache(int address);
    int getHits();
    int getMisses();
    Set* getSet(int i);
    int getNoOfSets();
    int getHitTime();
    int getMissPenalty();
    int getLoadLatency();
    int getStoreLatency();

    //For multiLevelCache
    int kickedOutAddress(int addr);
    void inValidateCacheLine(int addr);
};

#endif // CACHE_H
