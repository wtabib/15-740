#include <stdio.h>
#include "pin.H"
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache_tool.h"
#include <assert.h>

#define     N_CORES     5

FILE *trace;

int mem_access = 0;
int hits = 0;
int miss = 0;
int invalidate = 0;

std::vector<dirEntry> directoryCache;
std::vector<Cache> coreCaches;
std::vector<int> cores;
OS_THREAD_ID pid;

PIN_LOCK lock;

int find(dirEntry d) {
    unsigned int i = 0;
    for (i = 0; i < directoryCache.size(); i++) {
        if (directoryCache[i] == d) {
            return (int) i;
        }
    }   
    return -1;
}

/*int findTID(const OS_THREAD_ID &tid) {
    unsigned int i = 0;
    for (i = 0; i < tids.size(); i++) {
        if (tids[i] == tid) {
            return (int) i;
        }
    }   
    return -1;
}*/

VOID AddressR(ADDRINT iaddr, ADDRINT op, UINT32 opSize, bool write) {

    int core = PIN_ThreadId() % N_CORES;
    PIN_GetLock(&lock, PIN_ThreadId());
    mem_access++;
    std::cout << "read from core " << core << std::endl;

    dirEntry d = dirEntry(core, iaddr);
    int found = find(d);

    if (found == -1) {
        miss++;
        directoryCache.push_back(d);
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
                    }
                    else {
                        directoryCache[found].coreStates[i].s = S;
                        //if we need to add a new core, then we'll add it with
                        //state S
                    }
                    break;
                case M:
                    if (core == directoryCache[found].coreStates[i].core) {
                        //stay here
                    }
                    else {
                        //the core will flush it's data so everyone else can
                        //see it
                        directoryCache[found].coreStates[i].s = S;
                    }
                    break;
                case S:
                    directoryCache[found].coreStates[i].s = S;
                    break;
                case I: 
                    if (core == directoryCache[found].coreStates[i].core) {
                        invalidate++;
                        miss++;
                        directoryCache[found].coreStates[i].s = S;
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
        }

    }

    
    /*Cache c = coreCaches[core];
    MSI_STATE currState;

    dirEntry d = dirEntry(core, (ADDRINT)iaddr);

    std::cout << d.tid <<  "-" << directoryCache[core].tid <<  ": READ ";

    switch(directoryCache[core].s) {
        case M:
            if (!(directoryCache[core].tid == (d.tid))) {
                directoryCache[core].s = S;
                directoryCache[core].tid = d.tid;
            }else{
            }
            break;
        case E:
            break;
        case S:
            //pass
            directoryCache[core].tid = d.tid; 
            break;
        case I:
            directoryCache[core].s = S;
            directoryCache[core].tid = d.tid;
            break;
    }
    currState = directoryCache[core].s;
    */
    PIN_ReleaseLock(&lock);

    //update dCacheCount 
    //dCacheCount(c, currState, addr, );


}   //printf("Hello: %p\n", addr);


VOID AddressW(ADDRINT addr, ADDRINT pc, UINT32 size, bool write) {

    int core =  PIN_ThreadId() % N_CORES;
    PIN_GetLock(&lock, PIN_ThreadId());
    std::cout << "write from core " << core << std::endl;
    mem_access++;

    dirEntry d = dirEntry(core, addr);
    int found = find(d);

    if (found == -1) {
        miss++;
        directoryCache.push_back(d);
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
                    }
                    else {
                        directoryCache[found].coreStates[i].s = I;
                        //if we need to add a new core, then we'll add it with
                        //state S
                    }
                    break;
                case M:
                    if (core == directoryCache[found].coreStates[i].core) {
                        //stay here
                    }
                    else {
                        //the core will flush it's data so everyone else can
                        //see it
                        directoryCache[found].coreStates[i].s = I;
                    }
                    break;
                case S:
                    if (core == directoryCache[found].coreStates[i].core) {
                        directoryCache[found].coreStates[i].s = M;
                    }
                    else {
                        directoryCache[found].coreStates[i].s = I;
                    }
                    break;
                case I: 
                    if (core == directoryCache[found].coreStates[i].core) {
                        invalidate++;
                        miss++;
                        directoryCache[found].coreStates[i].s = M;
                    }
                    else {
                        //
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
        }

    }


    /*
    dirEntry d = dirEntry(core, (ADDRINT)addr);
    core = core % N_CORES;

    int a = 0;
    std::cout << d.tid <<  "-" << directoryCache[core].tid <<  ": WRITE ";
    switch(directoryCache[core].s) {
        case M:
            //pass
            //std::cout << directoryCache[it].mref << " M->M" << std::endl;
            break;
        case S:
            //std::cout << directoryCache[it].mref << " S->M" << std::endl;
            directoryCache[core].s = M;
            directoryCache[core].tid = d.tid;
            break;
        case I:
            //std::cout << directoryCache[it].mref << " I->M" << std::endl;
            directoryCache[core].s = M;
            directoryCache[core].tid = d.tid;
            break;
    } 
    */
    PIN_ReleaseLock(&lock);
} 

VOID Instruction(INS ins, VOID *v) {

    //std::cout << t << std::endl;
    const ADDRINT iaddr = INS_Address(ins);

    if (INS_IsMemoryRead(ins)) { 
        INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) AddressR,
                IARG_MEMORYREAD_EA, IARG_ADDRINT, iaddr, IARG_UINT32, INS_MemoryReadSize(ins),
                IARG_BOOL, false, IARG_END);

        if (INS_HasMemoryRead2(ins)) {
          INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)AddressR, IARG_MEMORYREAD2_EA,
                  IARG_ADDRINT, iaddr, IARG_UINT32, INS_MemoryReadSize(ins), IARG_BOOL,
                  false, IARG_END);
          }
    } 

    if (INS_IsMemoryWrite(ins)) { 
        INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) AddressW,
                IARG_MEMORYWRITE_EA, IARG_ADDRINT, iaddr,
                IARG_UINT32, INS_MemoryWriteSize(ins), IARG_BOOL, true, IARG_END);
    } 
}

VOID Fini(INT32 code, VOID *v) { 
    fclose(trace);
    for(int i= 0; i < directoryCache.size(); i++){
        directoryCache[i].prettyPrint();
        if(i>1000){
            break;
        }
    }
    std::cout << "hello" << std::endl;
    std::cout << "memory accesses = " << mem_access << std::endl;
    std::cout << "misses = " << miss << std::endl;
    std::cout << "invaliate = " << invalidate << std::endl;

}

void caches_init() {
    for (int i = 0; i < N_CORES; i++) 
    {
        cores.push_back(i);

        Cache c;
        int directMapped = 1; //set associative = 2;
        int cacheSize = 8; //cache size in kilobytes;
        int lineSize = 64; //line size in bytes;
        int missCycles = 100; //miss penalty in cycles

        c.update(directMapped, cacheSize*1024, lineSize, missCycles);

        coreCaches.push_back(c);
        //std::cout << "pid: " << pid << "\ttid: " << t << std::endl;
    }
}

int main(int argc, char *argv[]) { 

    //caches_init();

    PIN_InitSymbols();

    if (PIN_Init(argc, argv)) {
        std::cout << "pin error" << std::endl;
        return -1;
    }

    trace = fopen("pinatrace.out", "w");
    INS_AddInstrumentFunction(Instruction, 0); 
    PIN_AddFiniFunction(Fini, 0);
    PIN_InitLock(&lock);
    std::cout << "before start progam " << std::endl;
    PIN_StartProgram();
    return 0; 
}
