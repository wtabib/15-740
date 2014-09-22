#include "pin.H"
#include <math.h>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <bitset>


/*
   struct Block {
   int tag;
   char * data;
   };

   struct Cache {
   int numSets;
   int assoc;
   int cacheLine;
   Block *mem;
   int *lru;
   };

   bool addToCache(ADDRINT iaddr, Cache *cache) {

   }*/

#define MAX_SETS    256

class Set {
    public:
        int tag[2];
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

        Cache(int associativity, int cache_size, int line_size, int cache_miss) {
            assoc = associativity;
            cacheSize = cache_size;
            lineSize = line_size;
            numSets = (cache_size/line_size)/associativity;
            cacheMissPenalty = cache_miss;
            indexBits = log2(numSets);
            blockBits = log2(lineSize);
            tagBits  = 64 - indexBits - blockBits;
        }

        bool addToCache(ADDRINT iaddr) {
            long int maskTag = (long int) 0x1 << 63;
            maskTag = maskTag >> (tagBits-1);

            long int maskIndex = (long int) 0x1 << 63;
            maskIndex = maskIndex >> (tagBits + indexBits - 1);
            long int intermediateMask = maskIndex;
            maskIndex = maskTag ^ maskIndex;

            long int maskBlock = (long int) -1;
            maskBlock = maskBlock ^ intermediateMask;

            std::bitset<64> a(maskTag);
            std::bitset<64> b(maskIndex);
            std::bitset<64> c(maskBlock);

            std::cout << "tag mask\t\t" << a << std::endl;
            std::cout << "index mask\t\t" << b << std::endl;
            std::cout << "block mask\t\t" << c << std::endl;

            return true;
        }
        
};
