#include <iostream>

//static __attribute__((used)) int var1;
int main(){

    //asm("xchgb %b0,%1" :"=q" (oldval), "=m" (lock->lock) : "0" (0) : "memory");
    //asm("push eax");
    //asm("mov %0, 0x30" : "=r" (a) :);
    //asm("xchg %0,%1" :"=q" (a) : "r" (b) : "memory");
    //asm("pop");
    int out = 0;
    int var2 = 0;
    
    int var1 = 5;

    std::cout << var1 << " " << out << std::endl;
    int newval;
    asm("lab1: mov %0, %1" : "=r" (var1) : "r" (out) );
    std::cout << var1 << std::endl;
    asm("cmp %0, %1" : "=r" (var1) : "r" (var2)) ;
    asm("jne lab1");
    //asm volatile("lock; xchgl %0, %1" : "=r" (var1), "=m" (*out2) :  "0" (var1), "m"(&out2) :  "memory");
//    asm("xchg %0,%1" : "=r" (var1) : "=m" (*out) : "0"(1), "m"(*out) : "memory" );
    //asm("mov %1, %0" : "=r" (out) : "r" (var1));
    //std::cout << a << "," << b << std::endl;
    //std::cout << var1 << " " << out << newval << std::endl;

    return 0;
}
