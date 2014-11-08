#include <stdio.h>
#include "pin.H"
#include <iostream>
FILE *trace;
VOID Address(VOID * addr, VOID * tid) {
    fprintf(trace,"%p\t%p\n", addr, tid);
}

VOID Instruction(INS ins, VOID *v) {
    OS_THREAD_ID t;
    t = PIN_GetTid(); 
    std::cout << t << std::endl;
    if (INS_IsMemoryRead(ins)) { 
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(Address), 
        IARG_MEMORYREAD_EA, IARG_THREAD_ID, IARG_END); 
    } 
    if (INS_IsMemoryWrite(ins)) { 
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(Address), 
        IARG_MEMORYWRITE_EA, IARG_THREAD_ID, IARG_END); 
    } 
}

VOID Fini(INT32 code, VOID *v) { fclose(trace); }

int main(int argc, char *argv[]) { 
    PIN_Init(argc, argv);
    trace = fopen("pinatrace.out", "w");
    INS_AddInstrumentFunction(Instruction, 0); 
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();
    return 0; 
}
