/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2014 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include "pin.H"
#include <iostream>
#include <fstream>
#include <stdlib.h>

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

std::ofstream TraceFile;

/* ===================================================================== */
VOID BazArg(ADDRINT * arg1, ADDRINT * arg2, ADDRINT * arg3)
{
    TraceFile << "Baz(" << *arg1 << "," << *arg2 << "," << *arg3 << ")" << endl;
    *arg1 = 4;
    *arg2 = 5;
    *arg3 = 6;
}

VOID FooArg(ADDRINT * arg1)
{
    //std::cout << *arg1 << std::endl;
    std::cout << "address of a+3 in pin tool" << arg1 << std::endl;
    //*arg1 = 5;
    asm("lock incl (%0)" : : "r" (arg1) : "0");
    //__asm
    //{ 
    //    push eax
    //    mov eax, [arg1]
    //    inc dword ptr [eax]
    //    pop eax
    //};
}

/* ===================================================================== */

VOID Image(IMG img, VOID *v)
{
   RTN bazRtn = RTN_FindByName(img, "_Z3baziii");
    if (RTN_Valid(bazRtn))
    {
        RTN_Open(bazRtn);
        RTN_InsertCall(bazRtn, IPOINT_BEFORE, AFUNPTR(BazArg), IARG_FUNCARG_ENTRYPOINT_REFERENCE, 0, IARG_FUNCARG_ENTRYPOINT_REFERENCE, 1, IARG_FUNCARG_ENTRYPOINT_REFERENCE, 2, IARG_END);
        RTN_Close(bazRtn);
    }
   RTN fooRtn = RTN_FindByName(img, "_Z3fooPi");
    if (RTN_Valid(fooRtn))
    {
        RTN_Open(fooRtn);
        RTN_InsertCall(fooRtn, IPOINT_BEFORE, AFUNPTR(FooArg), IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);
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

    TraceFile.open("args.out");

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
