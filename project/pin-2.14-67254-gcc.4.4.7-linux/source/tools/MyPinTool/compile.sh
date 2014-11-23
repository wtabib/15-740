g++ $2.cpp -o $2.o -lpthread

PINEXEC=$1
rm ${PINEXEC}.so
g++ -DBIGARRAY_MULTIPLIER=1 -g -Wall -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../../source/include/pin -I../../../source/include/pin/gen -I../../../extras/components/include -I../../../extras/xed2-intel64/include -I../../../source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing   -c -o ${PINEXEC}.o ${PINEXEC}.cpp
g++ -shared -Wl,--hash-style=sysv -Wl,-Bsymbolic -Wl,--version-script=../../../source/include/pin/pintool.ver    -o  ${PINEXEC}.so ${PINEXEC}.o  -L../../../intel64/lib -L../../../intel64/lib-ext -L../../../intel64/runtime/glibc -L../../../extras/xed2-intel64/lib -lpin -lxed -ldwarf -lelf -ldl

../../../pin.sh -injection child -t ${PINEXEC}.so -- ./$2.o
