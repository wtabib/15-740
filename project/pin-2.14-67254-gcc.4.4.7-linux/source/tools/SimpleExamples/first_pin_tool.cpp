#include "pin.H"
#include "pin_profile.H"
#include <iostream>

int memoryInstructions = 0;

VOID instruction(INS ins, void *v) 
{

    if (INS_IsMemoryRead(ins)) {
        memoryInstructions ++;
    }
    return ;
}

VOID fini(int code, VOID *v) 
{
    std::cout << "I'm a survivor" << std::endl;
    std::cout << memoryInstructions << std::endl;
}

VOID ImageLoad( IMG img, VOID *v )
{
    // See if malloc() is present in the image.  If so, replace it.
    //
    RTN rtn = RTN_FindByName( img, "test" );
    if (RTN_Valid(rtn)){
        std::cout << "runnig test" << std::endl;
    }
}


int main(int argc, char *argv[])
{

    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        std::cout << "you fucked up" << std::endl;
    }


    //INS_AddInstrumentFunction(instruction, 0);

    IMG_AddInstrumentFunction( ImageLoad, 0 );
   
    //PIN_AddFiniFunction(fini, 0);
    // Start the program in probe mode, never returns
    //
    PIN_StartProgramProbed();

    //PIN_StartProgram();

    return 0;
}


