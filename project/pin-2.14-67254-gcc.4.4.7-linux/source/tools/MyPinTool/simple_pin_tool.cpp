#include <stdio.h>
#include "pin.H"
#include "pin_isa.H"
#include "pin_profile.H"
#include "xed-common-hdrs.h"
#include <iostream>
#include <fstream>

int mem_count = 0;

VOID Instruction(INS ins, void * v)
{
    if (INS_IsMemoryRead(ins)){
        mem_count++ ;
        //std::cout << "MEM READ" << std::endl;
    }
}


VOID Fini(int code, VOID * v){
    std::cout << "mem read: " << mem_count << std::endl;
}


int main(int argc, char *argv[])
{
    PIN_InitSymbols();

    if(! PIN_Init(argc,argv))
    {

        INS_AddInstrumentFunction(Instruction, 0);
        PIN_AddFiniFunction(Fini, 0);

    // Never returns

        PIN_StartProgram();
    
    }
    return 0;
}


/* ===================================================================== */
/* eof */
/* ===================================================================== */
