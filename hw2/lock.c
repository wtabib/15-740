#include <stdio.h>

void atomicIncr(int * location) {
	asm("lock incl (%0)" : : "r" (location) : "0");
}

int main() {
  int x = 0;
  atomicIncr(&x);
  printf("%d", x);
  return 0;
}
