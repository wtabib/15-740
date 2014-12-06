#include <iostream>

int baz(int a, int b, int c){
    return a+b+c;
}

void foo(int* a){
    return ;
}



int main(){
    std::cout<< "Hello" << std::endl;
    int one = baz(1,2,3);
    std::cout << one <<std::endl;
    int a[6]={0,0,0,0,0,0};
    std::cout << "address of a+3 = " << a+3 << std::endl;
    foo(a+3);
    std::cout << a[0] << a[1] << a[2] << a[3] << a[4] << std::endl;
    //int two = baz(4,5,6);
    return 0;
}

