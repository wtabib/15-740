#include <stdio.h>
#include "pin.H"
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache_tool.h"
#include <assert.h>
#include <string>

const string function = "call 0x40070d";

VOID Instruction(INS ins, VOID *v) {

    //std::cout << t << std::endl;
    const ADDRINT iaddr = INS_Address(ins);


    bool found = false;
    int thread =  PIN_ThreadId();
    if(INS_IsCall(ins)){
            //std::cout << INS_Disassemble(ins) << std::endl;
            //std::cout << function << "\n" << std::endl;
        if (!function.compare(INS_Disassemble(ins))) {
            std::cout << "found a call! " << INS_Disassemble(ins) << std::endl;
        }
    }
}

VOID Fini(INT32 code, VOID *v) { 
}

int main(int argc, char *argv[]) { 

    //caches_init();

    PIN_InitSymbols();

    if (PIN_Init(argc, argv)) {
        std::cout << "pin error" << std::endl;
        return -1;
    }

    INS_AddInstrumentFunction(Instruction, 0); 
    PIN_AddFiniFunction(Fini, 0);
    std::cout << "before start progam " << std::endl;
    PIN_StartProgram();
    return 0; 
}
