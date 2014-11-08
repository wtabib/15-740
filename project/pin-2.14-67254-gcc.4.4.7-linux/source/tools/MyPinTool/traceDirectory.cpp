#include <stdio.h>
#include "pin.H"
#include <iostream>
#include <algorithm>
#include <vector>
FILE *trace;

enum MSI_STATE {
    M,
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
            std::cout << "tid: " << tid << "\tmref: " << mref << "\ts: " << s << std::endl;
        }   
};


std::vector<dirEntry> cache;
int find(dirEntry d) {
    unsigned int i = 0;
    for (i = 0; i < cache.size(); i++) {
        if (cache[i] == d) {
            return (int) i;
        }
    }   
    return -1;
}


VOID AddressR(VOID * addr, uint32_t tid) {
    //fprintf(trace,"%p\t%u\n", addr, tid);
    //ADDRINT a = (ADDRINT) addr;
    // std::cout << "Address: " << a <<"\tProc: " <<  tid << std::endl;
    dirEntry d = dirEntry(tid, (ADDRINT)addr);

    int it = find(d);
     if (it > -1) {
            std::cout << d.tid << "-" << cache[it].tid <<  ": READ ";
            switch(cache[it].s) {
                case M:
                    if (!(cache[it].tid == (d.tid))) {
                        cache[it].s = S;
                        cache[it].tid = d.tid;
                        std::cout << cache[it].mref << " M->S" << std::endl;
                    }else{
                        std::cout << cache[it].mref << " M->M" << std::endl;
                    }
                    break;
                case S:
                    //pass
                    std::cout << cache[it].mref << " S->S" << std::endl;
                    cache[it].tid = d.tid; 
                    break;
                case I:
                    std::cout << cache[it].mref << " I->S" << std::endl;
                    cache[it].s = S;
                    cache[it].tid = d.tid;
                    break;
            }
    }
    else {
        cache.push_back(d);
    }
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
        std::cout << d.tid <<  "-" << cache[it].tid <<  ": WRITE ";
        switch(cache[it].s) {
            case M:
                //pass
                std::cout << cache[it].mref << " M->M" << std::endl;
                break;
            case S:
                std::cout << cache[it].mref << " S->M" << std::endl;
                cache[it].s = M;
                cache[it].tid = d.tid;
                break;
            case I:
                std::cout << cache[it].mref << " I->M" << std::endl;
                cache[it].s = M;
                cache[it].tid = d.tid;
                break;
       } 
    }
    else {
        cache.push_back(d);
    }
} 

VOID Instruction(INS ins, VOID *v) {
    OS_THREAD_ID t;
    t = PIN_GetTid(); 
    //std::cout << t << std::endl;
    if (INS_IsMemoryRead(ins)) { 
        /*dirEntry d = dirEntry(t, IARG_MEMORYREAD_EA);
        int it = find(d);
        if (it > -1) {
            std::cout << d.tid << ": READ ";
            switch(cache[it].s) {
                case M:
                    if (!(cache[it].tid == (d.tid))) {
                        cache[it].s = S;
                        std::cout << cache[it].mref << " M->S" << std::endl;
                    }else{
                        std::cout << cache[it].mref << " M->M" << std::endl;
                    }
                    break;
                case S:
                    //pass
                    std::cout << cache[it].mref << " S->S" << std::endl;
                    break;
                case I:
                    std::cout << cache[it].mref << " I->S" << std::endl;
                    cache[it].s = S;
                    break;
            }
        }
        else {
            cache.push_back(d);
        }*/
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(AddressR), 
                IARG_MEMORYREAD_EA, IARG_UINT32, t, IARG_END); 
    } 
    if (INS_IsMemoryWrite(ins)) { 
        /*dirEntry d = dirEntry(t, IARG_MEMORYWRITE_EA);
        int it = find(d);
        if (it > -1) {
            std::cout << d.tid << ": WRITE ";
            switch(cache[it].s) {
                case M:
                    //pass
                    std::cout << cache[it].mref << " M->M" << std::endl;
                    break;
                case S:
                    std::cout << cache[it].mref << " S->M" << std::endl;
                    cache[it].s = M;
                    cache[it].tid = d.tid;
                    break;
                case I:
                    std::cout << cache[it].mref << " I->M" << std::endl;
                    cache[it].s = M;
                    cache[it].tid = d.tid;
                    break;
            }
                e}
        else {
            cache.push_back(d);
        }*/
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(AddressW), 
                IARG_MEMORYWRITE_EA, IARG_UINT32, t, IARG_END); 
    } 
}

VOID Fini(INT32 code, VOID *v) { 
    fclose(trace);
    for(int i= 0; i < cache.size(); i++){
        cache[i].prettyPrint();
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
