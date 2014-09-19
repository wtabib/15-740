#include <stdio.h>
#include <string.h>


int main() {
  char mystring[10];
  char mystring2[10];
  mystring[7] = 'a';
  mystring[8] = 'b';
  mystring[9] = '\0';

  mystring2[0] = 'c';
  mystring[1] = 'd';
  mystring[2] = '\0';


  strcat(mystring+7, mystring2);
  printf("%s", mystring+7);
}
 
