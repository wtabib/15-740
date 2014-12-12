#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache_tool.h"
#include <assert.h>
#include "directoryCache.h"



int mem_access = 0;
int hits = 0;
int miss = 0;
int invalidate = 0;

std::vector<dirEntry> directoryCache;
std::vector<int> cores;

int find(dirEntry d) {
    unsigned int i = 0;
    for (i = 0; i < directoryCache.size(); i++) {
        if (directoryCache[i] == d) {
            return (int) i;
        }
    }   
    return -1;
}

//VOID AddressR(ADDRINT iaddr, ADDRINT op, UINT32 opSize, bool write) {
int updateDirectoryRead(int pid, int*addr, int WAIT_TIME, int BUS_TRANSFER, int LOOKUP, int FLUSH) {

    int num_clock_cycles = 0;

    int core = pid;
    mem_access++;
    //std::cout << "read from core " << core << std::endl;

    dirEntry d = dirEntry(core, addr);
    int found = find(d);

    if (found == -1) {
        miss++;
        directoryCache.push_back(d);

        //add the amount of time it takes to get the data from memory
        num_clock_cycles+=50;
    }
    else {
        //update the directory cache so that this core owns the block
        
        dirEntry dEntry = directoryCache[found];
        bool  foundCoreInEntry = false;
        for (int i = 0; i < directoryCache[found].coreStates.size(); i++) {
            if (core == directoryCache[found].coreStates[i].core) {
                foundCoreInEntry = true;
            }
        }

        for (int i = 0;  i < directoryCache[found].coreStates.size(); i++) { 
            switch(directoryCache[found].coreStates[i].s) {
                case E:
                    if (core == directoryCache[found].coreStates[i].core) {
                       //the cores are the same and they are in the exclusive
                       //state so don't change anything 
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    else {
                        directoryCache[found].coreStates[i].s = S;
                        //if we need to add a new core, then we'll add it with
                        //state S
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    break;
                case M:
                    if (core == directoryCache[found].coreStates[i].core) {
                        //stay here
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    else {
                        //the core will flush it's data so everyone else can
                        //see it
                        directoryCache[found].coreStates[i].s = S;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
                    }
                    break;
                case S:
                    directoryCache[found].coreStates[i].s = S;
                    num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    break;
                case I: 
                    if (core == directoryCache[found].coreStates[i].core) {
                        invalidate++;
                        miss++;
                        directoryCache[found].coreStates[i].s = S;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
                    }
                    break;
            }
        }

        if (foundCoreInEntry == false) {
            coreState s;
            s.core = core;
            s.s = S;
            directoryCache[found].coreStates.push_back(s);
            miss++;
            num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
        }

    }

    
    return num_clock_cycles;

}


//VOID AddressW(ADDRINT addr, ADDRINT pc, UINT32 size, bool write) {
int updateDirectoryWrite(int pid, int*addr, int WAIT_TIME, int BUS_TRANSFER, int LOOKUP, int FLUSH) {
    int num_clock_cycles = 0;
    int core =  pid;
    //std::cout << "write from core " << core << std::endl;
    mem_access++;

    dirEntry d = dirEntry(core, addr);
    int found = find(d);

    if (found == -1) {
        miss++;
        directoryCache.push_back(d);
        num_clock_cycles += 50;
    }
    else {
        //update the directory cache so that this core owns the block
        
        dirEntry dEntry = directoryCache[found];
        bool  foundCoreInEntry = false;
        for (int i = 0; i < directoryCache[found].coreStates.size(); i++) {
            if (core == directoryCache[found].coreStates[i].core) {
                foundCoreInEntry = true;
            }
        }

        for (int i = 0;  i < directoryCache[found].coreStates.size(); i++) { 
            switch(directoryCache[found].coreStates[i].s) {
                case E:
                    if (core == directoryCache[found].coreStates[i].core) {
                       //the cores are the same and they are in the exclusive
                       //state so don't change anything 
                        directoryCache[found].coreStates[i].s = M;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    else {
                        directoryCache[found].coreStates[i].s = I;
                        //if we need to add a new core, then we'll add it with
                        //state S
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    break;
                case M:
                    if (core == directoryCache[found].coreStates[i].core) {
                        //stay here
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    else {
                        //the core will flush it's data so everyone else can
                        //see it
                        directoryCache[found].coreStates[i].s = I;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
                    }
                    break;
                case S:
                    if (core == directoryCache[found].coreStates[i].core) {
                        directoryCache[found].coreStates[i].s = M;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    else {
                        directoryCache[found].coreStates[i].s = I;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }
                    break;
                case I: 
                    if (core == directoryCache[found].coreStates[i].core) {
                        invalidate++;
                        miss++;
                        directoryCache[found].coreStates[i].s = M;
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
                    }
                    else {
                        //
                        num_clock_cycles+=2*BUS_TRANSFER+LOOKUP;
                    }

                    break;
            }
        }

        if (foundCoreInEntry == false) {
            coreState s;
            s.core = core;
            s.s = M;
            directoryCache[found].coreStates.push_back(s);
            miss++;
            num_clock_cycles+=2*BUS_TRANSFER+LOOKUP+FLUSH;
        }

    }

    return num_clock_cycles;

} 
