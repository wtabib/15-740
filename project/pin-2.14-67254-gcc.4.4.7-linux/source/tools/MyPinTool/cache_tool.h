#include "pin.H"
#include <math.h>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <bitset>


#define MAX_SETS    256

enum MSI_STATE {
    M,
    E,
    S,
    I
};

class dirEntry {
    public:
        OS_THREAD_ID    tid;
        ADDRINT         mref;
        MSI_STATE       s;

        dirEntry(OS_THREAD_ID t, ADDRINT a){
            tid = t;
            mref = a;
            s = S;
        }

        bool operator==(const dirEntry &other){
            return mref == other.mref;
        } 

        void prettyPrint()  {
            //std::cout << "tid: " << tid << "\tmref: " << mref << "\ts: " << s << std::endl;
        }   
};

struct PC {
    bool isLoad; 
    unsigned int refs;
    unsigned int miss;
    unsigned int loadMisses;
    unsigned int storeMisses;
    ADDRINT pc;
};

bool myfunction (PC i, PC j) {
    return (i.miss > j.miss);
}

class Set {
    public:
        ADDRINT tag[2];
        int lru;
};

class Cache {

    public:
        int numSets;
        int assoc;
        int cacheLine;
        Set sets[MAX_SETS];
        int *lru;
        int cacheSize;
        int lineSize;
        int cacheMissPenalty;
        int tagBits;
        int indexBits;
        int blockBits;

        Cache() {}

        void update(int associativity, int cache_size, int line_size, int cache_miss) {
            assoc = associativity;
            cacheSize = cache_size;
            lineSize = line_size;
            numSets = (cache_size/line_size)/associativity;
            cacheMissPenalty = cache_miss;
            indexBits = log2(numSets);
            blockBits = log2(lineSize);
            tagBits  = 64 - indexBits - blockBits;
        }

        //this function gets an instruction, determines if it's a hit or miss, and then puts the new data in the cache
        bool doCacheStuff(ADDRINT iaddr) {
            long int maskTag = (long int) 0x1 << 63;
            maskTag = maskTag >> (tagBits-1);

            long int maskIndex = (long int) 0x1 << 63;
            maskIndex = maskIndex >> (tagBits + indexBits - 1);
            long int intermediateMask = maskIndex;
            maskIndex = maskTag ^ maskIndex;

            long int maskBlock = (long int) -1;
            maskBlock = maskBlock ^ intermediateMask;

#ifdef DEBUG
            std::bitset<64> a(maskTag);
            std::bitset<64> b(maskIndex);
            std::bitset<64> c(maskBlock);

            std::cout << "tag mask\t\t" << a << std::endl;
            std::cout << "index mask\t\t" << b << std::endl;
            std::cout << "block mask\t\t" << c << std::endl;
#endif


            ADDRINT iAddrTag = maskTag & iaddr;
            ADDRINT iAddrIndex = ((ADDRINT)maskIndex & iaddr) >> (blockBits);
            ADDRINT iAddrBlock = maskBlock & iaddr;

#ifdef DEBUG
            std::bitset<64> d(iAddrTag);
            std::bitset<64> e(iAddrIndex);
            std::bitset<64> f(iAddrBlock);
            std::bitset<64> g(iaddr);

            std::cout << "iaddr\t\t\t" << g << std::endl;
            std::cout << "tag\t\t\t" << d << std::endl;
            std::cout << "index\t\t\t" << e << std::endl;
            std::cout << "block\t\t\t" << f << "\n\n" << std::endl;
#endif

            Set *targetSet = &sets[iAddrIndex];
            for (int i = 0; i < assoc; i++) {

                if (targetSet->tag[i] == iAddrTag) {

                    //this is a hit

                    targetSet->lru = (i+1)%assoc;
                    return true;
                }
            }

            //this is a miss
            targetSet->tag[targetSet->lru] = iAddrTag;
            targetSet->lru = (targetSet->lru+1)%assoc;
            return false;
        }
        
};
