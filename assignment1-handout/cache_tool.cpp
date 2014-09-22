#include "pin.H"
#include <iostream>
#include <fstream>

#include "pin_profile.H"

#include "cache_tool.h"

std::ofstream outFile;
long long int counter = 0;
long long int iCacheMiss = 0;
Cache c;

VOID countInstruction(ADDRINT iaddr)
{
    counter++;
    bool hit = c.doCacheStuff(iaddr);
    if (!hit) {
        iCacheMiss++;
    }
}

VOID instruction(INS ins, void *v) 
{
    const ADDRINT iaddr = INS_Address(ins);

    INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR) countInstruction,
            IARG_ADDRINT,
            iaddr,
            IARG_END);


}

VOID fini(int code, VOID *v) 
{
    std::cout << "INSTRUCTION EXECUTION\t\t\t" << counter << std::endl;
    std::cout << "CACHE MISSES\t\t\t" << iCacheMiss << std::endl;
}


int main(int argc, char *argv[])
{

    c.update(2, 8*1024, 64, 0);
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


