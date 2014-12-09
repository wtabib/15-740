#include "pin.H"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

std::ofstream TraceFile;

VOID FooArg(ADDRINT * arg1, ADDRINT * lock)
{

    //std::cout << *lock << std::endl;
    int result = 1;
    int var2 = 0;
    asm volatile( "enter_region: lock;"
            "xchg %0, %1;"
            : "=r"(result), "=m"(*lock)
            : "0"(result), "m"(*lock)
            : "memory");
    asm volatile("cmp %%rbx, %%rax" : "=b"(result) : "b" (result), "a" (var2));
    asm volatile("jne enter_region");

    //ADD HERE YOUR CODE
    //std::cout << "Before: " << *arg1 << std::endl; 
    *arg1=*arg1+1;
    //std::cout << "After: " << *arg1 << std::endl; 

    //asm volatile("mov %0, %1" : "=r" (*lock) : "r" (var2));
    *lock = 0;

    //asm("lock incl (%0)" : : "r" (arg1) : "0");
    //std::cout << *lock << std::endl;
   
}

/* ===================================================================== */

VOID Image(IMG img, VOID *v)
{
   RTN fooRtn = RTN_FindByName(img, "_Z3fooPiS_");
    if (RTN_Valid(fooRtn))
    {
        RTN_Open(fooRtn);
        RTN_InsertCall(fooRtn, IPOINT_BEFORE, AFUNPTR(FooArg), IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_FUNCARG_ENTRYPOINT_VALUE, 1, IARG_END);
        RTN_Close(fooRtn);
    }


}

/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
    TraceFile.close();
}

/* ===================================================================== */

int main(int argc, char *argv[])
{
    PIN_InitSymbols();
    PIN_Init(argc, argv);

    TraceFile.open("pintoolASM.out");

    TraceFile << hex;
    TraceFile.setf(ios::showbase);

    IMG_AddInstrumentFunction(Image, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
