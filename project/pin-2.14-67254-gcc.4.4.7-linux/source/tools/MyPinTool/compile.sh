#g++ -c -I../InstLib -fomit-frame-pointer -Wall -Werror -Wno-unknown-pragmas  -O3 -fomit-frame-pointer -MMD -DBIGARRAY_MULTIPLIER=1 -DUSING_XED  -fno-strict-aliasing -I../Include -I../InstLib -I../../../extras/xed2-intel64/include -I../../../extras/components/include -I../../../source/include -I../../../source/include/gen -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -O3 -fomit-frame-pointer -o obj-intel64/$1.o $1.cpp 
#g++  -Wl,--hash-style=sysv -shared -Wl,-Bsymbolic -Wl,--version-script=../../../source/include/pintool.ver -L../Lib/ -L../ExtLib/ -L../../../extras/xed2-intel64/lib -L../../../intel64/lib -L../../../intel64/lib-ext  -o obj-intel64/$1.so obj-intel64/$1.o -L../Lib/ -L../ExtLib/ -L../../../extras/xed2-intel64/lib -L../../../intel64/lib -L../../../intel64/lib-ext -lpin  -lxed -ldwarf -lelf -ldl  

g++ $2.cpp -o $2.o;

PINEXEC=$1
rm ${PINEXEC}.so
g++ -DBIGARRAY_MULTIPLIER=1 -g -Wall -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../../source/include/pin -I../../../source/include/pin/gen -I../../../extras/components/include -I../../../extras/xed2-intel64/include -I../../../source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing   -c -o ${PINEXEC}.o ${PINEXEC}.cpp
g++ -shared -Wl,--hash-style=sysv -Wl,-Bsymbolic -Wl,--version-script=../../../source/include/pin/pintool.ver    -o  ${PINEXEC}.so ${PINEXEC}.o  -L../../../intel64/lib -L../../../intel64/lib-ext -L../../../intel64/runtime/glibc -L../../../extras/xed2-intel64/lib -lpin -lxed -ldwarf -lelf -ldl

/home/vittorio/Downloads/pin-2.14-67254-gcc.4.4.7-linux/pin -injection child -t ${PINEXEC}.so -- ./$2.o
