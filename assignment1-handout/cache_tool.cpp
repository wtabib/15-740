#include "pin.H"
#include <iostream>
#include <fstream>

#include "pin_profile.H"

#include "cache_tool.h"

std::ofstream outFile;
long long int counter = 0;
long long int iCacheMiss = 0;
long long int dCacheMiss = 0;
Cache iCache, dCache;

VOID iCacheCount(ADDRINT iaddr)
{
    counter++;
    //TODO: memory vs. instruction
    //add data structure to assign blame
    int numBytes = 16;
    for (int i = 0; i < numBytes; i++) {
        bool hit = iCache.doCacheStuff(iaddr+i);
        if (!hit) {
            iCacheMiss++;
        }
    }

}

VOID dCacheCount(ADDRINT iaddr, ADDRINT op, UINT32 opSize)
{
    UINT32 numBytes = opSize;
    for (unsigned int i = 0; i < numBytes; i++) {
        bool hit = dCache.doCacheStuff(op+i);
        if (!hit) {
            dCacheMiss++;
        }
    }
}

VOID instruction(INS ins, void *v) 
{

    UINT32 numOperands = INS_OperandCount(ins);
    int numOpMems = 0;
    for (unsigned int i = 0; i < numOperands; i++) {
        //check number of reads and writes
        if (INS_OperandIsMemory(ins, i))
            numOpMems++;
    }

    if (numOpMems > 2) {
        std::cout << "number of operands = " << numOpMems << std::endl;
    }

    const ADDRINT iaddr = INS_Address(ins);

    INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR) iCacheCount,
            IARG_ADDRINT,
            iaddr,
            IARG_END);

    if (INS_IsMemoryRead(ins)) {
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                IARG_ADDRINT,
                iaddr,
                IARG_MEMORYREAD_EA,
                IARG_MEMORYREAD_SIZE,
                IARG_END);

        if (INS_HasMemoryRead2(ins)) {
            INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                    IARG_ADDRINT,
                    iaddr,
                    IARG_MEMORYREAD2_EA,
                    IARG_MEMORYREAD_SIZE,
                    IARG_END);


        }

    }
    if (INS_IsMemoryWrite(ins)) {
         INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                IARG_ADDRINT,
                iaddr,
                IARG_MEMORYWRITE_EA,
                IARG_MEMORYWRITE_SIZE,
                IARG_END);
    }

}

VOID fini(int code, VOID *v) 
{
    //long long int total = counter + iCacheMiss + dCacheMiss;
    //double insP, iCacheP, dCacheP;
    std::cout << "INSTRUCTION EXECUTION\t\t\t" << counter << std::endl;
    std::cout << "iCACHE MISSES\t\t\t\t" << iCacheMiss << std::endl;
    std::cout << "dCACHE MISSES\t\t\t\t" << dCacheMiss << std::endl;
}


int main(int argc, char *argv[])
{

    iCache.update(2, 8*1024, 64, 0);
    dCache.update(2, 8*1024, 64, 0);
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


