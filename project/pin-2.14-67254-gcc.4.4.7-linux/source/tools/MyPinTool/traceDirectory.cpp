#include <stdio.h>
#include "pin.H"
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache_tool.h"
#include <assert.h>

FILE *trace;


std::vector<dirEntry> directoryCache;
std::vector<Cache> threadCaches;
std::vector<OS_THREAD_ID> tids;
OS_THREAD_ID pid;
int find(dirEntry d) {
    unsigned int i = 0;
    for (i = 0; i < directoryCache.size(); i++) {
        if (directoryCache[i] == d) {
            return (int) i;
        }
    }   
    return -1;
}

int findTID(const OS_THREAD_ID &tid) {
    unsigned int i = 0;
    for (i = 0; i < tids.size(); i++) {
        if (tids[i] == tid) {
            return (int) i;
        }
    }   
    return -1;
}

VOID AddressR(ADDRINT iaddr, ADDRINT op, UINT32 opSize, /*UINT32 idx,*/ UINT32 isLoad, UINT32 isSecondOp, uint32_t tid ) {
    //fprintf(trace,"%p\t%u\n", addr, tid);
    //ADDRINT a = (ADDRINT) addr;
    // std::cout << "Address: " << a <<"\tProc: " <<  tid << std::endl;
    
    OS_THREAD_ID t;
    t = PIN_GetTid(); 
    pid = PIN_GetPid();
    
    int idx = findTID(t);
    std::cout << "t = " << t << std::endl;
    assert(idx != -1);

    Cache c = threadCaches[idx];
    MSI_STATE currState;
 
    dirEntry d = dirEntry(tid, (ADDRINT)iaddr);

    int it = find(d);
     if (it > -1) {
            //std::cout << d.tid << "-" << directoryCache[it].tid <<  ": READ ";
            switch(directoryCache[it].s) {
                case M:
                    if (!(directoryCache[it].tid == (d.tid))) {
                        directoryCache[it].s = S;
                        directoryCache[it].tid = d.tid;
                        //std::cout << directoryCache[it].mref << " M->S" << std::endl;
                    }else{
                        //std::cout << directoryCache[it].mref << " M->M" << std::endl;
                    }
                    break;
                case E:
                    break;
                case S:
                    //pass
                    //std::cout << directoryCache[it].mref << " S->S" << std::endl;
                    directoryCache[it].tid = d.tid; 
                    break;
                case I:
                    //std::cout << directoryCache[it].mref << " I->S" << std::endl;
                    directoryCache[it].s = S;
                    directoryCache[it].tid = d.tid;
                    break;
            }
        currState = directoryCache[it].s;
    }
    else {
        directoryCache.push_back(d);
        currState = S;
    }

    //update dCacheCount 
    //dCacheCount(c, currState, addr, );

   
}   //printf("Hello: %p\n", addr);


VOID AddressW(VOID * addr, uint32_t tid) {
    //fprintf(trace,"%p\t%p\n", addr, tid);
    //printf("Hello: %p\n", addr);
    //OS_THREAD_ID t;
    //t = PIN_GetTid(); 
    dirEntry d = dirEntry(tid, (ADDRINT)addr);

    int it = find(d);
    if (it > -1) {
        int a = 0;
        //std::cout << d.tid <<  "-" << directoryCache[it].tid <<  ": WRITE ";
        switch(directoryCache[it].s) {
            case M:
                //pass
                //std::cout << directoryCache[it].mref << " M->M" << std::endl;
                break;
            case S:
                //std::cout << directoryCache[it].mref << " S->M" << std::endl;
                directoryCache[it].s = M;
                directoryCache[it].tid = d.tid;
                break;
            case I:
                //std::cout << directoryCache[it].mref << " I->M" << std::endl;
                directoryCache[it].s = M;
                directoryCache[it].tid = d.tid;
                break;
       } 
    }
    else {
        directoryCache.push_back(d);
    }
} 

VOID Instruction(INS ins, VOID *v) {
    OS_THREAD_ID t;
    t = PIN_GetTid(); 
    pid = PIN_GetPid();
    
    if (! (std::find(tids.begin(), tids.end(), t) != tids.end()) )
    {
        //then the tid needs to be put into the tid array
        tids.push_back(t);

        Cache c;
        int directMapped = 1; //set associative = 2;
        int cacheSize = 8; //cache size in kilobytes;
        int lineSize = 64; //line size in bytes;
        int missCycles = 100; //miss penalty in cycles

        c.update(directMapped, cacheSize*1024, lineSize, missCycles);

        threadCaches.push_back(c);
        std::cout << "pid: " << pid << "\ttid: " << t << std::endl;
    }

    
    //std::cout << t << std::endl;
    const ADDRINT iaddr = INS_Address(ins);

    if (INS_IsMemoryRead(ins)) { 
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) AddressR,
                IARG_ADDRINT,
                iaddr,
                IARG_MEMORYREAD_EA,
                IARG_MEMORYREAD_SIZE,
                IARG_UINT32,
                1,
                IARG_UINT32,
                0,
                IARG_UINT32,
                t,
                IARG_END);

        /*INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(AddressR), 
                IARG_MEMORYREAD_EA, IARG_UINT32, t, IARG_END); */

        /*if (INS_HasMemoryRead2(ins)) {
            INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(AddressR), 
                IARG_MEMORYREAD2_EA, IARG_UINT32, t, IARG_END); 
        }*/
    } 
    
    if (INS_IsMemoryWrite(ins)) { 
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(AddressW), 
                IARG_MEMORYWRITE_EA, IARG_UINT32, t, IARG_END); 
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

}

int main(int argc, char *argv[]) { 
    /*dirEntry d1 = dirEntry(777, 400);
      dirEntry d2 = dirEntry(777, 400);
      bool same = (d1 == d2);
      std::cout << "are they the same " << same << std::endl;*/

    PIN_Init(argc, argv);
    trace = fopen("pinatrace.out", "w");
    INS_AddInstrumentFunction(Instruction, 0); 
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();
    return 0; 
}
