#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void penter(void)
{
    //void *ra = _ReturnAddress();
    printf("hello\n");
}

void penter2(void) 
{

}

int main() {

    printf("more hello\n");
    //penter();
    //asm(" call _Z6penterv;");
    penter();

    return 0;
}
