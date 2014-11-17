#include "pin.H"
#include <iostream>
#include <fstream>

#include "pin_profile.H"

#include "cache_tool.h"
#include <algorithm>
#include <vector>

std::ofstream outFile;
long long int counter = 0;
long long int iCacheMiss = 0;
long long int dCacheMiss = 0;
Cache iCache, dCache;
std::vector<PC> dCachePC, iCachePC;

KNOB<UINT32> KnobLineSize(KNOB_MODE_WRITEONCE, "pintool", 
        "lz", "64", "line size in bytes");

KNOB<UINT32> KnobCacheSize(KNOB_MODE_WRITEONCE, "pintool", 
        "cz", "8", "cache size in kilobytes");

KNOB<UINT32> KnobMissCycles(KNOB_MODE_WRITEONCE, "pintool", 
        "mp", "100", "miss penalty in cycles");

KNOB<UINT32> KnobAssoc(KNOB_MODE_WRITEONCE, "pintool", 
        "a", "1", "1 is direct-mapped, 2 is set associative");

VOID iCacheCount(ADDRINT iaddr, UINT32 idx)
{
    counter++;
    //TODO: memory vs. instruction
    //add data structure to assign blame
    int numBytes = 16;
    iCachePC[idx].refs++;
    for (int i = 0; i < numBytes; i++) {
        bool hit = iCache.doCacheStuff(iaddr+i);
        if (!hit) {
            iCachePC[idx].miss++;
            iCacheMiss++;
        }
    }
}

// if isLoad == 0 it's a store, if isLoad == 1 it's a load
VOID dCacheCount(Cache &c, const MSI_STATE &s, ADDRINT iaddr, ADDRINT op, UINT32 opSize, UINT32 idx, UINT32 isLoad, UINT32 isSecondOp)
{
    UINT32 numBytes = opSize;
    if (! isSecondOp)
        dCachePC[idx].refs++;
    for (unsigned int i = 0; i < numBytes; i++) {
        bool hit = dCache.doCacheStuff(op+i);
        if (!hit) {
            dCachePC[idx].miss++;
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

    UINT32 idx;
   struct PC item;
   item.refs = 0;
   item.miss = 0;
   item.pc = iaddr;
   iCachePC.push_back(item);
   idx = iCachePC.size() - 1;


    INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR) iCacheCount,
            IARG_ADDRINT,
            iaddr,
            IARG_UINT32,
            idx,
            IARG_END);

    if (INS_IsMemoryRead(ins)) {
        struct PC readItem;
        readItem.refs = 0;
        readItem.miss = 0;
        readItem.pc = iaddr;
        readItem.isLoad = 1;
        dCachePC.push_back(readItem);
        UINT32 idx2 = dCachePC.size()-1;
        INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                IARG_ADDRINT,
                iaddr,
                IARG_MEMORYREAD_EA,
                IARG_MEMORYREAD_SIZE,
                IARG_UINT32,
                idx2,
                IARG_UINT32,
                1,
                IARG_UINT32,
                0,
                IARG_END);

        if (INS_HasMemoryRead2(ins)) {
            INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                    IARG_ADDRINT,
                    iaddr,
                    IARG_MEMORYREAD2_EA,
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32,
                    idx2,
                    IARG_UINT32,
                    1,
                    IARG_UINT32,
                    1,
                    IARG_END);
        }

    }
    if (INS_IsMemoryWrite(ins)) {
        struct PC writeItem;
        writeItem.refs = 0;
        writeItem.miss = 0;
        writeItem.pc = iaddr;
        writeItem.isLoad = 0;
        dCachePC.push_back(writeItem);
        UINT32 idx2 = dCachePC.size()-1;

         INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR) dCacheCount,
                IARG_ADDRINT,
                iaddr,
                IARG_MEMORYWRITE_EA,
                IARG_MEMORYWRITE_SIZE,
                IARG_UINT32,
                idx2,
                IARG_UINT32,
                0,
                IARG_UINT32,
                0,
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

    ////////////////////// iCache printing /////////////////////////////
    std::cout << "\niCACHE" << std::endl;
    std::sort(iCachePC.begin(), iCachePC.end(), myfunction);
    std::cout << "pc \t\t refs \t\t miss\t\t contribution \t total cycles \t miss rate" << std::endl;
    for (int i = 0; i < iCachePC.size(); i++) {
        double contribution = iCachePC[i].miss/((double)iCacheMiss);
        double missRate = iCachePC[i].miss/((double)iCachePC[i].refs);
        std::cout << ((void *) iCachePC[i].pc) << "\t" << iCachePC[i].refs << "\t" <<
            iCachePC[i].miss << "\t" << contribution << "%\t" << iCachePC[i].miss*KnobMissCycles.Value()<< "\t" << missRate << "%\t" << std::endl;
        //TODO: replace the above 1 with number of cycles
    }

    ////////////////////// dCache printing /////////////////////////////
    std::cout << "\ndCACHE" << std::endl;
    std::sort(dCachePC.begin(), dCachePC.end(), myfunction);
    std::cout << "pc \t\t refs \t\t isLoad? \t\t miss\t\t contribution \t total cycles \t miss rate" << std::endl;
    for (int i = 0; i < dCachePC.size(); i++) {
        double contribution = dCachePC[i].miss/((double)dCacheMiss);
        double missRate = dCachePC[i].miss/((double)dCachePC[i].refs);
        std::cout << ((void *) dCachePC[i].pc) << "\t" << dCachePC[i].refs << "\t" <<
            dCachePC[i].isLoad << "\t" << dCachePC[i].miss << "\t" << contribution << "%\t" << dCachePC[i].miss*KnobMissCycles.Value()<< "\t" << missRate << "%\t" << std::endl;
        //TODO: replace the above 1 with number of cycles
    }

}


int main(int argc, char *argv[])
{

    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        std::cout << "you fucked up" << std::endl;
    }

    iCache.update(KnobAssoc.Value(), KnobCacheSize.Value()*1024, KnobLineSize.Value(), KnobMissCycles.Value());
    dCache.update(KnobAssoc.Value(), KnobCacheSize.Value()*1024, KnobLineSize.Value(), KnobMissCycles.Value());

    //outFile.open(KnobOutputFile.Value().c_str());

    INS_AddInstrumentFunction(instruction, 0);
    PIN_AddFiniFunction(fini, 0);

    PIN_StartProgram();

    return 0;
}


