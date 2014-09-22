#include "pin.H"
#include <iostream>
#include <fstream>

#include "pin_profile.H"

#include "cache_tool.h"

std::ofstream outFile;
long long int counter = 0;

VOID countInstruction()
{
    counter++;
}

VOID instruction(INS ins, void *v) 
{
    //const ADDRINT iaddr = INS_Address(ins);
    //const UINT32 instId = profile.Map(iaddr);

    INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR) countInstruction,
            IARG_END);


}

VOID fini(int code, VOID *v) 
{
    std::cout << "INSTRUCTION EXECUTION\t\t\t" << counter << std::endl;
}


int main(int argc, char *argv[])
{

    Cache c(2, 64, 8, 0);
    ADDRINT iaddr = 0;
    c.addToCache(iaddr); 
    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        std::cout << "you fucked up" << std::endl;
    }


    //outFile.open(KnobOutputFile.Value().c_str());

    INS_AddInstrumentFunction(instruction, 0);
    PIN_AddFiniFunction(fini, 0);

    PIN_StartProgram();

    return 0;
}


