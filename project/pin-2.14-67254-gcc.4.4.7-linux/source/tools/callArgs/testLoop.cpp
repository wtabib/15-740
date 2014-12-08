#include <iostream>
#include <stdlib.h>
#include <stdio.h>

//static __attribute__((used)) int var1;
int main(){

    int var2 = 0;
    //int *var1 = (int *) malloc(sizeof(int));
    int var1 = 5;
    //int var1 = 5;

    asm volatile("lab1:");
    var2++;
    std::cout << var2 << std::endl;
    std::cout << var1 << std::endl;
    asm volatile("cmp %%rbx, %%rax; jne lab1;" :  : "a" (var2), "b" (var1)) ;
    //asm volatile("jne lab1");

    std::cout << "end" << std::endl;
    std::cout << var2 << std::endl;
    return 0;
}
